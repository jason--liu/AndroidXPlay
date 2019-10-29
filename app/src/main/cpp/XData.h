//
// Created by jason on 19-10-14.
//

#ifndef XPLAY_XDATA_H
#define XPLAY_XDATA_H

struct XData {
    unsigned char *data = 0;
    unsigned char *datas[8] = {0};//存放解码后的数据
    int width = 0;
    int height = 0;
    int size = 0;
    bool isAudio = false;

    void Drop();
};

#endif //XPLAY_XDATA_H
