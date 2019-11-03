//
// Created by jason on 19-11-3.
//

#ifndef XPLAY_IAUDIOPLAY_H
#define XPLAY_IAUDIOPLAY_H


#include "IObserver.h"
#include "XParameter.h"

class IAudioPlay : public IObserver {
public:
    // 接收重采样后的数据
    virtual void Update(XData data);

    virtual bool StartPlay(XParameter out) = 0;
};


#endif //XPLAY_IAUDIOPLAY_H
