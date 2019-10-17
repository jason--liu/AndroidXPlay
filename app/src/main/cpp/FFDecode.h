//
// Created by jason on 19-10-17.
//

#ifndef XPLAY_FFDECODE_H
#define XPLAY_FFDECODE_H


#include "IDecode.h"
struct AVCodecContext;
class FFDecode : public IDecode {
public:
    virtual bool Open(XParameter para);

protected:
    AVCodecContext *CodecContext = nullptr;
};


#endif //XPLAY_FFDECODE_H
