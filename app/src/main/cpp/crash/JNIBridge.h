//
// Created by 陈鹏 on 2021/8/22.
//

#ifndef MONITOR_JNIBRIDGE_H
#define MONITOR_JNIBRIDGE_H


#include <jni.h>
#include "CrashAnalyser.h"

class JNIBridge {
private:
    JavaVM *javaVm;
    jobject callbackObj;
    jclass nativeCrashMonitorClass;
public:
    JNIBridge(JavaVM *javaVm, jobject callbackObj, jclass nativeCrashMonitorClass);
public:
    void throwException2Java(struct native_handler_context_struct *handlerContext);
};


#endif //MONITOR_JNIBRIDGE_H













