//
// Created by jason on 19-10-14.
//

#ifndef XPLAY_IDEMUX_H
#define XPLAY_IDEMUX_H

#include "XData.h"
class IDemux {
public:
    // 打开文件或者流媒体 rtmp http rtsp
    virtual bool Open(const char *url) = 0;
    // 读取1帧数据，数据由调用者清理
    virtual XData Read()=0;

    int totalMS = 0;
};


#endif //XPLAY_IDEMUX_H
