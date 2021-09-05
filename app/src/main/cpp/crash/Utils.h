//
// Created by 陈鹏 on 2021/9/5.
//

#ifndef MONITOR_UTILS_H
#define MONITOR_UTILS_H
#include <signal.h>
#include <stdlib.h>

extern const char* desc_sig(int sig, int code);
extern const char* getProcessName(pid_t pid);
extern const char* getThreadName(pid_t tid);
extern bool is_dll(const char* dll_name);



#endif //MONITOR_UTILS_H
