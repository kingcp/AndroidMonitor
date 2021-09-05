//
// Created by 陈鹏 on 2021/8/22.
//

#ifndef MONITOR_CRASHANALYSER_H
#define MONITOR_CRASHANALYSER_H

#include <signal.h>
#include "CrashDefine.h"

typedef struct native_handler_context_struct{
    int code;
    siginfo_t *si;
    void *sc;
    pid_t pid;
    pid_t tid;
    const char *processName;
    const char *threadName;
    uintptr_t frames[BACKTRACE_FRAMES_MAX];
    int frame_size;

} native_handler_context;
extern void initCondition();
void* threadCrashMonitor(void *argv);
extern void waitForSignal();
extern void analysisNativeException();
extern void notifyCaughtSignal(int code, siginfo_t *si,void *sc);
extern void copyInfo2Context(int code, siginfo_t *si, void *sc);


#endif //MONITOR_CRASHANALYSER_H
