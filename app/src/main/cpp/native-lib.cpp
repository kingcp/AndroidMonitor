#include <jni.h>
#include <string>
#include <pthread.h>
#include "dlopen.h"
#include "crash/JNIBridge.h"
#include "crash/CrashAnalyser.h"
#include "crash/CrashDefine.h"
#include "crash/SignalHandler.h"


extern "C" jstring
Java_com_king_cp_monitor_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    //获取thread的锁对象，通过锁对象获取当前持有的thread
    //mirror::Object* monitor::GetContendMonitor(Thread* thread)
    //void *so_addr = ndk_dlopen("libart.so");

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_king_cp_monitor_crash_NativeCrashMonitor_nativeInit(JNIEnv *env,jobject nativeCrashMonitor,
                                                             jobject callback) {
    //主要是把 callback 保存起来，方便监听到异常时回调给 java 层
    callback = env->NewGlobalRef(callback);
    JavaVM *javaVm;
    env->GetJavaVM(&javaVm);
    // 为了避免子线程为空的情况
    jclass nativeCrashMonitorClass = env->GetObjectClass(nativeCrashMonitor);
    nativeCrashMonitorClass = (jclass)env->NewGlobalRef(nativeCrashMonitorClass);
    JNIBridge *jniBridge = new JNIBridge(javaVm,callback,nativeCrashMonitorClass);
    // 创建一个线程去监听是否有异常
    initCondition();
    pthread_t pthread;
    int ret = pthread_create(&pthread, nullptr,threadCrashMonitor,jniBridge);
    if(ret){
        LOGE("pthread_create error, ret: %d",ret);
    }
    LOGE("pthread_create success, ret: %d",ret);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_king_cp_monitor_crash_NativeCrashMonitor_nativeSetup(JNIEnv *env,jobject thiz) {
    // 设置监听信号量回调处理
    installSignalHandlers();
    // 设置额外的栈空间，让信号处理在单独的栈中处理
    installAlternateStack();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_king_cp_monitor_crash_NativeCrashMonitor_nativeCrash(JNIEnv* env,jobject thiz) {
    LOGE("NativeCrashMonitor_nativeCrash");
    int *num = NULL;
    *num = 100;
}






























