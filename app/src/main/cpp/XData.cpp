//
// Created by jason on 19-10-14.
//

#include "XData.h"
extern "C"{
#include <libavformat/avformat.h>
}
void XData::Drop() {
    if(!data) return;
    av_packet_free((AVPacket**)&data);
    data = nullptr;
    size = 0;
}