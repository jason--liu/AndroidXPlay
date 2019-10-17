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
    // 3.打开解码器
    int re = avcodec_open2(CodecContext, cd, NULL);
    if(re !=0){
        char buf[1024]={0};
        av_strerror(re, buf, sizeof(buf)-1);
        XLOGE("%s", buf);
        return false;
    }
    XLOGI("avcodec open success");
    return true;
}