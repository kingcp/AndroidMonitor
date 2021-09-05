package com.king.cp.monitor.crash

interface CrashHandlerListener {
    fun onCrash(threadName:String,error:Error)
}