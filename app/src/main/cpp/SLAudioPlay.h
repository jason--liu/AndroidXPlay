//
// Created by jason on 19-11-3.
//

#ifndef XPLAY_SLAUDIOPLAY_H
#define XPLAY_SLAUDIOPLAY_H


#include "IAudioPlay.h"

class SLAudioPlay : public IAudioPlay {
public:
    virtual bool StartPlay(XParameter out);

    void PlayCall(void *bufq);
    SLAudioPlay();
    virtual ~SLAudioPlay();

protected:
    uint8_t *buf = 0;
};


#endif //XPLAY_SLAUDIOPLAY_H
