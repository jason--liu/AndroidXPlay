//
// Created by jason on 19-10-17.
//

#ifndef XPLAY_IDECODE_H
#define XPLAY_IDECODE_H

#include "XParameter.h"
#include "IObserver.h"
#include <list>

// 解码接口，支持硬解码
class IDecode : public IObserver {
public:
    //打开解码器
    virtual bool Open(XParameter para) = 0;

    // 发生数据到线程解码（立即返回）
    virtual bool SendPacket(XData pkt) = 0;

    virtual XData ReceiveFrame() = 0;

    // 由主体notify的数据(阻塞)
    virtual void Update(XData pkt);

    bool isAudio = false;
    int maxList = 100;//缓冲4S左右
protected:
    virtual void Main();

    // 读取缓冲
    std::list<XData> packs;
    std::mutex packsMutex;

};


#endif //XPLAY_IDECODE_H
