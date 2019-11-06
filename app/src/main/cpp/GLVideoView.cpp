//
// Created by jason on 19-10-28.
//

#include "GLVideoView.h"
#include "XTexture.h"
#include "XLog.h"

void GLVideoView::SetRender(void *win) {
    view = win;
}

void GLVideoView::Render(XData data) {
    if (!view) return;
    if (!Xtxt) {
        Xtxt = XTexture::Create();
        XLOGD("GLVideoView Render %d",data.format);
        Xtxt->Init(view, (XTextureType)data.format);
    }
    Xtxt->Draw(data.datas, data.width, data.height);
}