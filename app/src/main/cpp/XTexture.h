//
// Created by jason on 19-10-28.
//

#ifndef XPLAY_XTEXTURE_H
#define XPLAY_XTEXTURE_H


class XTexture {
public:
    virtual bool Init(void *win) = 0;

    virtual void Draw(unsigned char *data[], int width, int height) = 0;

    static XTexture *Create();
};


#endif //XPLAY_XTEXTURE_H
