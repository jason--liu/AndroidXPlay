//
// Created by jason on 19-11-3.
//

#ifndef XPLAY_FFRESAMPLE_H
#define XPLAY_FFRESAMPLE_H


#include "IResample.h"

struct SwrContext;

class FFResample: public IResample {
public:
    virtual bool Open(XParameter in, XParameter out = XParameter());
    virtual XData Resample(XData indata);
private:
    SwrContext *actx = nullptr;
};


#endif //XPLAY_FFRESAMPLE_H
