//
// Created by jason on 19-11-3.
//

#ifndef XPLAY_IAUDIOPLAY_H
#define XPLAY_IAUDIOPLAY_H


#include "IObserver.h"
#include "XParameter.h"
#include <list>

class IAudioPlay : public IObserver {
public:
    // 接收重采样后的数据
    // 缓冲满后阻塞
    virtual void Update(XData data);

    virtual bool StartPlay(XParameter out) = 0;

    // 获取缓冲数据，如没有则阻塞
    virtual XData GetData();

    // 最大缓冲
    int maxFrame = 100;

private:
    std::list<XData> frames;
    std::mutex framesMux;
};


#endif //XPLAY_IAUDIOPLAY_H
