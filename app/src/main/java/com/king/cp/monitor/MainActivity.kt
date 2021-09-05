package com.king.cp.monitor

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.king.cp.monitor.crash.CrashHandlerListener
import com.king.cp.monitor.crash.NativeCrashMonitor

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        findViewById<TextView>(R.id.sample_text).text = stringFromJNI()

        val nativeCrashMonitor = NativeCrashMonitor()

        nativeCrashMonitor.init(object:CrashHandlerListener{
            override fun onCrash(threadName: String, error: Error) {
                nativeCrashMonitor.getStackInfoByThreadName(threadName)
            }
        })
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}