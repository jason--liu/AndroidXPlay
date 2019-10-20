//
// Created by jason on 19-10-17.
//

#include "IDecode.h"
#include "XLog.h"

void IDecode::Update(XData pkt) {
    if (pkt.isAudio != isAudio) {
        return;;
    }
    while (!isExit) {
        std::lock_guard<std::mutex> lock(packsMutex);
        // 阻塞
        if (packs.size() < maxList) {
            packs.push_back(pkt);
            break;
        }
        XSleep(1);
    }
    std::lock_guard<std::mutex> lock(packsMutex);
    // 阻塞
    if (packs.size() > maxList) {

    }
    packs.push_back(pkt);

}

void IDecode::Main() {
    while (!isExit) {
        std::lock_guard<std::mutex> lock(packsMutex);
        if (packs.empty()) {
            XSleep(1);
            continue;
        }
        //取出packet
        XData pack = packs.front();
        packs.pop_front();//从链表中删掉

        // 发生数据到解码线程，一个packet可能多解出来多个frame
        if (this->SendPacket(pack)) {
            while (!isExit) {
                // 获取解码数据
                XData frame = ReceiveFrame();
                if (!frame.data) break;
                XLOGD("RecvFrame %d", frame.size);
                // 发生数据给观察者
                this->Notify(frame);
            }
        }
        pack.Drop();
    }
}