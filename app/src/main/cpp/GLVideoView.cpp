//
// Created by jason on 19-10-28.
//

#include "GLVideoView.h"
#include "XTexture.h"

void GLVideoView::SetRender(void *win) {
    view = win;
}

void GLVideoView::Render(XData data) {
    if (!view) return;
    if (!Xtxt) {
        Xtxt = XTexture::Create();
        Xtxt->Init(view);
    }
    Xtxt->Draw(data.datas, data.width, data.height);
}