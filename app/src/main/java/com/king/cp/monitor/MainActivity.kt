package com.king.cp.monitor

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.widget.Button
import android.widget.TextView
import com.king.cp.monitor.crash.CrashHandlerListener
import com.king.cp.monitor.crash.JavaCrashMonitor
import com.king.cp.monitor.crash.NativeCrashMonitor
import java.lang.NullPointerException

class MainActivity : AppCompatActivity() {

    lateinit var mCrashMonitorBtn:Button
    lateinit var mNeverCrashBtn:Button
    lateinit var mJavaCrashBtn:Button
    lateinit var mNativeCrashBtn:Button


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val nativeCrashMonitor = NativeCrashMonitor()

        mCrashMonitorBtn = findViewById(R.id.crash_monitor_btn)
        mCrashMonitorBtn.setOnClickListener {
            nativeCrashMonitor.init(object:CrashHandlerListener{
                override fun onCrash(threadName: String, error: Error) {
                    NativeCrashMonitor.getStackInfoByThreadName(threadName)
                }
            })
        }

        mJavaCrashBtn = findViewById(R.id.java_crash_monitor_btn)
        mJavaCrashBtn.setOnClickListener {
            JavaCrashMonitor.initJavaCrashMonitor()
        }

        mNeverCrashBtn = findViewById(R.id.never_crash_btn)
        mNeverCrashBtn.setOnClickListener {
            Handler(Looper.getMainLooper()).post{
                while (true){
                    try {
                        Looper.loop()
                    }catch (e : Exception ){
                        e.printStackTrace()
                    }
                }

            }
        }

        mJavaCrashBtn = findViewById(R.id.java_crash_btn)
        mJavaCrashBtn.setOnClickListener {
            throw NullPointerException()
        }

        mNativeCrashBtn = findViewById(R.id.native_crash_btn)
        mNativeCrashBtn.setOnClickListener {
            Log.e("MainActivity","mNeverCrashBtn onClick")
            NativeCrashMonitor.nativeCrash()
        }


    }
}