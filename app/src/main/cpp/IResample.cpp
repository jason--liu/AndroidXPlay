//
// Created by jason on 19-11-3.
//

#include "IResample.h"
#include "XLog.h"

void IResample::Update(XData data) {
    XData d = this->Resample(data);
    if (d.size > 0) {
        this->Notify(d);
    }
}