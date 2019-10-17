//
// Created by jason on 19-10-17.
//

#include "XThread.h"
#include "XLog.h"
#include <thread>

void XSleep(int ms) {
    std::chrono::milliseconds du(ms);
    std::this_thread::sleep_for(du);
}

//using namespace一般不放在头文件，避免冲突
void XThread::Start() {
    isExit = false;
    std::thread th(&XThread::ThreadMain, this);
    //当前线程放弃对新创建线程控制
    th.detach();
}

void XThread::ThreadMain() {
    XLOGI("enter ThreadMain");
    isRunning = true;
    Main();
    XLOGI("out of ThreadMain");
    isRunning = false;
}

void XThread::Stop() {
    isExit = true;
    for (int i = 0; i < 200; i++) {
        if (!isRunning){
            XLOGD("Stop 停止线程成功");
            return;
        }
        XSleep(1);
    }
    XLOGI("stop 线程超时");
}
