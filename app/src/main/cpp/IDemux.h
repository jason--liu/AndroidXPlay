//
// Created by jason on 19-10-14.
//

#ifndef XPLAY_IDEMUX_H
#define XPLAY_IDEMUX_H

#include "XData.h"
#include "XThread.h"
#include "IObserver.h"
#include "XParameter.h"

class IDemux :public IObserver{
public:
    // 打开文件或者流媒体 rtmp http rtsp
    virtual bool Open(const char *url) = 0;
    // 获取视频参数
    virtual XParameter GetPara() = 0;
    // 读取1帧数据，数据由调用者清理
    virtual XData Read()=0;

    int totalMS = 0;
protected:
    virtual void Main();
};


#endif //XPLAY_IDEMUX_H
