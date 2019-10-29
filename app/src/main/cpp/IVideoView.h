//
// Created by jason on 19-10-28.
//

#ifndef XPLAY_IVIDEOVIEW_H
#define XPLAY_IVIDEOVIEW_H

#include "XData.h"

class IVideoView {
public:
    virtual void SetRender(void *win) = 0;

    virtual void Render(XData data) = 0;

    virtual void Update(XData data);
};


#endif //XPLAY_IVIDEOVIEW_H
