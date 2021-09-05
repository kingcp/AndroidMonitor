//
// Created by 陈鹏 on 2021/9/5.
//

#ifndef MONITOR_SIGNALHANDLER_H
#define MONITOR_SIGNALHANDLER_H

extern bool installSignalHandlers();// 设置监听信号量回调处理
extern void installAlternateStack();// 设置额外的栈空间，让信号处理在单独的栈中处理


#endif //MONITOR_SIGNALHANDLER_H
