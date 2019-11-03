//
// Created by jason on 19-11-3.
//

#include "IAudioPlay.h"
#include "XLog.h"

void IAudioPlay::Update(XData data) {
    // 压入缓冲队列
    if (data.size <= 0 || !data.data)
        return;
    while (!isExit) {
        framesMux.lock();
        if (frames.size() > maxFrame) {
            framesMux.unlock();
            XSleep(1);
            continue;
        }
        frames.push_back(data);
        framesMux.unlock();
        break;
    }
}
