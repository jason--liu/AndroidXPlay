//
// Created by jason on 19-11-3.
//

#include "IAudioPlay.h"
#include "XLog.h"

XData IAudioPlay::GetData() {
    XData d;
    while (!isExit) {
        framesMux.lock();
        if (!frames.empty()) {
            d = frames.front();
            frames.pop_front();
            framesMux.unlock();
            return d;
        }
        framesMux.unlock();
        XSleep(1);
    }
    return d;
}

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
