//
// Created by jason on 19-10-28.
//

#ifndef XPLAY_XTEXTURE_H
#define XPLAY_XTEXTURE_H

enum XTextureType {
    XTEXTURE_YUV420P = 0,// yyyy u v
    XTEXTURE_NV12 = 23, // yyyy uv
    XTEXTURE_NV21 = 24  //yyyy vu
};

class XTexture {
public:
    virtual bool Init(void *win, XTextureType type = XTEXTURE_YUV420P) = 0;

    virtual void Draw(unsigned char *data[], int width, int height) = 0;

    static XTexture *Create();
};


#endif //XPLAY_XTEXTURE_H
