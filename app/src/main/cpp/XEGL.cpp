//
// Created by jason on 19-10-28.
//

#include "XEGL.h"

class CXEGL : public XEGL {
public:
    virtual bool Init(void *win) {
        return true;
    }
};

XEGL *XEGL::Get() {
    static CXEGL egl;
    return &egl;
}