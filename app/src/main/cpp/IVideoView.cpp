//
// Created by jason on 19-10-28.
//

#include "IVideoView.h"

void IVideoView::Update(XData data) {
    this->Render(data);//Render由继承者实现
}