//
// Created by jason on 19-10-17.
//

#ifndef XPLAY_XPARAMETER_H
#define XPLAY_XPARAMETER_H


struct AVCodecParameters;

class XParameter {
public:
    AVCodecParameters *para = 0;
    int channels = 2; // 增加这个变量为了解耦
    int sample_rate = 441000;

};


#endif //XPLAY_XPARAMETER_H
