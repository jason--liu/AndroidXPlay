//
// Created by jason on 19-10-28.
//

#ifndef XPLAY_XEGL_H
#define XPLAY_XEGL_H


class XEGL {
public:
    virtual bool Init(void *win) = 0;//定义成void 就没有外部类型依赖
    virtual void Draw() = 0;

    static XEGL *Get();

protected:
    XEGL() {}
};


#endif //XPLAY_XEGL_H
