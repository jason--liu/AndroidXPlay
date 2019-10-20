//
// Created by jason on 19-10-17.
//

#ifndef XPLAY_FFDECODE_H
#define XPLAY_FFDECODE_H


#include "IDecode.h"

struct AVCodecContext;
struct AVFrame;

class FFDecode : public IDecode {
public:
    virtual bool Open(XParameter para);

    virtual bool SendPacket(XData pkt);

    virtual XData ReceiveFrame();

protected:
    AVCodecContext *CodecContext = nullptr;
    AVFrame *frame = nullptr;
};


#endif //XPLAY_FFDECODE_H
