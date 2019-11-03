//
// Created by jason on 19-11-3.
//

#include "FFResample.h"
#include "XLog.h"

extern "C" {
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}

bool FFResample::Open(XParameter in, XParameter out) {
    //if (in.para->format)
    // 音频重采样上下文初始化
    actx = swr_alloc();
    actx = swr_alloc_set_opts(actx, av_get_default_channel_layout(out.channels),
                              AV_SAMPLE_FMT_S16, out.sample_rate,
                              av_get_default_channel_layout((in.para->channels)),
                              static_cast<AVSampleFormat >(in.para->format), in.para->sample_rate,
                              NULL, NULL);
    int re = swr_init(actx);
    if (re != 0) {
        XLOGE("swr init failed");
        return false;
    }
    outChannels = in.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    return true;
}

XData FFResample::Resample(XData indata) {
    //XLOGD("indata size is %d", indata.size);
    if (indata.size <= 0 || !indata.data)
        return XData();
    if (!actx)
        return XData();

    XData out;
    AVFrame *frame = reinterpret_cast<AVFrame *>(indata.data);

    // 输出空间分配
    // size =channesl * samples * sample_size * 2
    int size = outChannels * frame->nb_samples *
               av_get_bytes_per_sample(static_cast<AVSampleFormat >(outFormat));
    //XLOGD("av_get_bytes_per_sample %d",
      //    av_get_bytes_per_sample(static_cast<AVSampleFormat >(outFormat))); ==2
      //XLOGD("frame->nb_samples %d",frame->nb_samples); 1024
    if (size <= 0)
        return XData();

    out.Alloc(size);
    uint8_t *outArr[2] = {0};
    outArr[0] = out.data;
    int len = swr_convert(actx, outArr, frame->nb_samples, (const uint8_t **) frame->data,
                          frame->nb_samples);
    if (len <= 0) {
        out.Drop();
        return XData();
    }
    //XLOGD("swr_convert %d", len);

    return out;
}
