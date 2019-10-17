//
// Created by jason on 19-10-17.
//

#include "XThread.h"
#include "XLog.h"
#include <thread>

//using namespace一般不放在头文件，避免冲突
void XThread::Start() {
    std::thread th(&XThread::ThreadMain, this);
    //当前线程放弃对新创建线程控制
    th.detach();
}

void XThread::ThreadMain() {
    XLOGI("enter ThreadMain");
    Main();
    XLOGI("out of ThreadMain");
}

void XThread::Stop() {

}
