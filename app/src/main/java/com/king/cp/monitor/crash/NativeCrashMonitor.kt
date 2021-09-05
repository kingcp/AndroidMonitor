package com.king.cp.monitor.crash

import android.os.Looper
import android.text.TextUtils
import android.util.Log


class NativeCrashMonitor {
    val TAG : String = "NATIVE_CRASH_MONITOR"

    lateinit var systemThreadGroup: ThreadGroup

    init {
        System.loadLibrary("native-lib")
        try {
            val threadGroupClass = Class.forName("java.lang.ThreadGroup")
            val systemThreadGroupField = threadGroupClass.getDeclaredField("systemThreadGroup")
            systemThreadGroupField.isAccessible = true
            systemThreadGroup = systemThreadGroupField.get(null) as ThreadGroup
        }catch (e : Exception){
        }
    }

    @Volatile
    private var isInit = false

    fun init(callback: CrashHandlerListener){
        if(isInit)
            return
        isInit = true
        nativeInit(callback)
        nativeSetup()
    }

    external fun nativeInit(callback: CrashHandlerListener)

    external fun nativeSetup()

    external fun nativeCrash()

    fun getStackInfoByThreadName(threadName:String):String{
        val thread = getThreadByName(threadName)
        val sb = StringBuffer()
        val stackTraceElements = thread?.stackTrace
        if (stackTraceElements != null) {
            for(stackTraceElement in stackTraceElements)
                sb.append(stackTraceElement.toString()).append("\r\n")
        }
        return sb.toString()
    }

    /**
     * 根据线程名字返回对应线程
     */
    private fun getThreadByName(threadName:String):Thread?{
        if(TextUtils.isEmpty(threadName))
            return null
        var theThread :Thread? = null
        if(threadName == "main"){
            theThread = Looper.getMainLooper().thread
        }else{
            var threadArray = arrayOf<Thread?>()
            try {
                val threadSet: Set<Thread?> = getAllStackTraces().keys
                threadArray = threadSet.toTypedArray()
            } catch (e: java.lang.Exception) {
                Log.e(TAG, "dump thread Traces", e)
            }
            for (thread in threadArray) {
                if (thread?.name == threadName) {
                    theThread = thread
                    Log.e(TAG, "find it.$threadName")
                }
            }

        }
        return theThread
    }


    /**
     * 获取所有存活的线程和堆栈信息
     */
    private fun getAllStackTraces(): Map<Thread?, Array<StackTraceElement>> {
        return if (systemThreadGroup == null) {
            Thread.getAllStackTraces()
        } else {
            val map: MutableMap<Thread?, Array<StackTraceElement>> = HashMap()

            // Find out how many live threads we have. Allocate a bit more
            // space than needed, in case new ones are just being created.
            var count = systemThreadGroup.activeCount()
            val threads = arrayOfNulls<Thread>(count + count / 2)
            Log.d(TAG, "activeCount: $count")

            // Enumerate the threads and collect the stacktraces.
            count = systemThreadGroup.enumerate(threads)
            for (i in 0 until count) {
                try {
                    map[threads[i]] = threads[i]!!.stackTrace
                } catch (e: Throwable) {
                    Log.e(TAG, "fail threadName: " + threads[i]!!.name, e)
                }
            }
            map
        }
    }

}























