package com.king.cp.monitor.crash


class JavaCrashMonitor {

    companion object{

        fun initJavaCrashMonitor(){
            val androidExceptionHandler: Thread.UncaughtExceptionHandler = Thread.getDefaultUncaughtExceptionHandler()
            Thread.setDefaultUncaughtExceptionHandler(JavaCrashDefaultUncaughtHandler(androidExceptionHandler = androidExceptionHandler))
        }
    }

    class JavaCrashDefaultUncaughtHandler (var androidExceptionHandler: Thread.UncaughtExceptionHandler):Thread.UncaughtExceptionHandler{
        override fun uncaughtException(t: Thread, e: Throwable) {
            println("原先处理Crash的类->${androidExceptionHandler}")
            // 自己处理java crash 信息
            e.printStackTrace()
            // 看需求交个原先的处理
            androidExceptionHandler.uncaughtException(t,e)

        }

    }
}
