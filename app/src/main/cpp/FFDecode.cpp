//
// Created by jason on 19-10-17.
//
extern "C" {
#include <libavcodec/avcodec.h>
}

#include "FFDecode.h"
#include "XLog.h"

bool FFDecode::Open(XParameter para) {
    if (!para.para)
        return false;
    AVCodecParameters *p = para.para;
    // 1.查找解码器
    AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if (!cd) {
        XLOGE("avcodec_find_decoder %d failed", p->codec_id);
        return false;
    }
    XLOGI("avcodec_find_decoder success");
    // 2.创建解码器上下文
    CodecContext = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(CodecContext, p);
    CodecContext->thread_count = 4;
    // 3.打开解码器
    int re = avcodec_open2(CodecContext, cd, NULL);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        XLOGE("%s", buf);
        return false;
    }
    XLOGI("avcodec open success");
    if (CodecContext->codec_type == AVMEDIA_TYPE_VIDEO) {
        this->isAudio = false;
    } else {
        this->isAudio = true;
    }
    return true;
}

bool FFDecode::SendPacket(XData pkt) {
    // 临时变量不存在线程安全问题
    if (pkt.size <= 0 || !pkt.data)
        return false;
    if (!CodecContext) {
        return false;
    }
    int re = avcodec_send_packet(CodecContext, reinterpret_cast<AVPacket *>(pkt.data));
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        XLOGE("%s", buf);
        return false;
    }

    return true;
}

XData FFDecode::ReceiveFrame() {
    if (!CodecContext) {
        return XData();
    }
    if (!frame) {
        frame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(CodecContext, frame);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        //XLOGE("avcodec_receive_frame %s", buf);
        return XData();
    }
    //解码成功
    XData d;
    d.data = reinterpret_cast<unsigned char *>(frame);
    //XLOGD("codec type %d", CodecContext->codec_type);
    if (CodecContext->codec_type == AVMEDIA_TYPE_VIDEO) {
        d.width = frame->width;
        d.height = frame->height;
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
        // XLOGD("d.size = %d", d.size);
    } else
        // 样本字节*单通道样本数*通道数
        d.size = av_get_bytes_per_sample(static_cast<AVSampleFormat>(frame->format)) *
                 frame->nb_samples * 2;//32位
    memcpy(d.datas, frame->data, sizeof(d.datas));
    return d;
}