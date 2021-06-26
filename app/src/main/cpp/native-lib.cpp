#include <jni.h>
#include <string>
#include "dlopen.h"

extern "C" jstring
Java_com_king_cp_monitor_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    //获取thread的锁对象，通过锁对象获取当前持有的thread
    //mirror::Object* monitor::GetContendMonitor(Thread* thread)
    void *so_addr = ndk_dlopen("libart.so");

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

