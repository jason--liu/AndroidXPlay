//
// Created by jason on 19-10-14.
//

#include "FFDemux.h"
#include "XLog.h"

extern "C"
{
#include <libavformat/avformat.h>
}

bool FFDemux::Open(const char *url) {
    XLOGI("Open file %s", url);
    int re = avformat_open_input(&ic, url, NULL, NULL);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("FFDemux open %s failed, error %s!", url, buf);
        return false;
    }
    XLOGI("FFDemux success");

    // 获取文件信息
    re = avformat_find_stream_info(ic, NULL);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("FFDemux avformat_find_stream_info %s failed", url);
        return false;
    }
    this->totalMS = ic->duration / (AV_TIME_BASE) / 1000;
    XLOGI("total ms = %d", this->totalMS);//1S中有多少个单位，除以1000换算成毫秒

    return true;
}

XData FFDemux::Read() {
    if (!ic) return XData();
    XData d;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if (re != 0) {
        av_packet_free(&pkt);
        return XData();
    }
    //XLOGI("pack size is %d ptss %lld", pkt->size,pkt->pts);

    d.data = reinterpret_cast<unsigned char *>(pkt);
    //d.data = (unsigned char *)pkt;
    d.size = pkt->size;
    return d;
}

FFDemux::FFDemux() {
    static bool isFirst = true;
    if (isFirst) {
        // 线程不安全
        isFirst = false;
        // 初始化网络
        //avformat_network_init();
        XLOGD("register ffmpeg");
    }
}

XParameter FFDemux::GetPara() {
    if (!ic) {
        XLOGE("ic is null");
        return XParameter();
    }
    // 获取视频流索引
    int Vindex = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (Vindex < 0) {
        XLOGE("find video stream failed");
        return XParameter();
    }
    XParameter para;
    para.para = ic->streams[Vindex]->codecpar;
    return para;
}