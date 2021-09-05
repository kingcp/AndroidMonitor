//
// Created by 陈鹏 on 2021/9/5.
//

#include <unistd.h>
#include "SignalHandler.h"
#include "CrashDefine.h"




void signalPass(int code, siginfo_t *si, void *sc){
    LOGE("监听到了 native 的崩溃");
    // 考虑非信号方式防止死锁
    signal(code,SIG_DFL);
    signal(SIGALRM,SIG_DFL);
    (void)alarm(8);
    // 解析栈信息，回调给java层
    notifyCaughtSignal(code,si,sc);
    // 给系统原来默认的处理，否则就会进入死循环
    oldHandlers[code].sa_sigaction(code,si,sc);
}

/**
 * int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact));
 * signum：代表信号编码，可以是除SIGKILL及SIGSTOP外的任何一个特定有效的信号，如果为这两个信号定义自己的处理函数，将导致信号安装错误。
 * act：指向结构体sigaction的一个实例的指针，该实例指定了对特定信号的处理，如果设置为空，进程会执行默认处理。
 * oldact：和参数act类似，只不过保存的是原来对相应信号的处理，也可设置为NULL。
 * @return
 */
bool installSignalHandlers(){
    // 需要保存原来的处理，获取系统或者其他第三方已经设置的
    for(int i = 0 ; i < exceptionSignalsNumber; ++i){
        if(sigaction(exceptionSignals[i],NULL,&oldHandlers[exceptionSignals[i]]) == -1)
            return false;
    }
    // 初始化赋值
    struct sigaction sa;
    memset(&sa,0,sizeof(sa));
    sigemptyset(&sa.sa_mask);
    // 指定信号处理的回调函数
    sa.sa_sigaction = signalPass;
    sa.sa_flags = SA_ONSTACK || SA_SIGINFO;
    // 处理当前信号量的时候不关心其他的
    for(int i = 0 ; i < exceptionSignalsNumber; i++){
        sigaddset(&sa.sa_mask,exceptionSignals[i]);
    }
    // 调用 sigaction 来处理信号回调
    for(int i = 0 ; i < exceptionSignalsNumber ; i++){
        if(sigaction(exceptionSignals[i],&sa,NULL) == -1){
            // 输出警告
            LOGD("异常信号处理存在问题");
        }
    }
    return true;
}
