//
// Created by jason on 19-10-14.
//

#ifndef XPLAY_FFDEMUX_H
#define XPLAY_FFDEMUX_H

#include "XData.h"
#include "IDemux.h"

struct  AVFormatContext;
// 解封装接口
class FFDemux : public IDemux {
public:
    // 打开文件或者流媒体 rtmp http rtsp
    virtual bool Open(const char *url);
    virtual XParameter GetPara();
    // 读取1帧数据，数据由调用者清理
    virtual XData Read();
    FFDemux();

private:
    // c++11 支持在类声明中初始化，但也只有在无参数的构造函数被调用时才生效
    AVFormatContext *ic = nullptr;
};


#endif //XPLAY_FFDEMUX_H
