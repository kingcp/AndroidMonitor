//
// Created by 陈鹏 on 2021/8/22.
//

#include <dlfcn.h>
#include "JNIBridge.h"
#include "CrashDefine.h"
#include "Utils.h"


JNIBridge::JNIBridge(JavaVM *javaVm, jobject callbackObj, jclass nativeCrashMonitorClass) {
    this->javaVm = javaVm;
    this->callbackObj = callbackObj;
    this->nativeCrashMonitorClass = nativeCrashMonitorClass;
}

void JNIBridge::throwException2Java(struct native_handler_context_struct *handlerContext) {
    LOGD("throwException2Java");
    // java的线程名 -> 当前Java的堆栈
    JNIEnv *env = NULL;
    if(this->javaVm->AttachCurrentThread(&env,NULL) != JNI_OK){
        LOGE("AttachCurrentThread failed!");
    }
    const char *sig = "(Ljava/lang/String;)Ljava/lang/String;";
    jmethodID getStackInfoByThreadNameMid = env->GetStaticMethodID(this->nativeCrashMonitorClass,"getStackInfoByThreadName",sig);
    jstring jThreadName = env->NewStringUTF(handlerContext->threadName);
    jobject javaStackInfo = env->CallStaticObjectMethod(this->nativeCrashMonitorClass,getStackInfoByThreadNameMid,jThreadName);
    const char *javaExceptionStackInfo = env->GetStringUTFChars(static_cast<jstring>(javaStackInfo), JNI_FALSE);
    LOGE("java stack : %s",javaExceptionStackInfo);
    // c++ 堆栈信息
    int frame_size = handlerContext->frame_size;
    for(int index = 0 ; index < frame_size; index++){
        uintptr_t pc = handlerContext->frames[index];
        Dl_info info;
        void *const addr = reinterpret_cast<void *const>(pc);
        if(dladdr(addr,&info) != 0 && info.dli_fname != NULL){
            const uintptr_t near = reinterpret_cast<const uintptr_t>(info.dli_saddr);
            const uintptr_t offs = pc - near;
            const uintptr_t addr_rel = pc - (uintptr_t)info.dli_fbase;
            const uintptr_t addr_to_use = is_dll(info.dli_fname) ? addr_rel : pc;
            // 使用addr2line 根据pc值获得在哪一行
            LOGD("native crash #%02lx pc-> Ox%016lx %s (%s+0x%lx)",index,addr_to_use,info.dli_fname,info.dli_sname,offs);
        }
    }
    //把 java 异常堆栈和 c++ 异常堆栈一起抛到回调监听函数

    if(this->javaVm->DetachCurrentThread() != JNI_OK){
        LOGE("");
    }
}










