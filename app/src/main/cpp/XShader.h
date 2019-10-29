//
// Created by jason on 19-10-28.
//

#ifndef XPLAY_XSHADER_H
#define XPLAY_XSHADER_H


class XShader {
public:
    virtual bool Init();

    virtual void GetTexture(unsigned int index, int width, int height, unsigned char *buf);//获取材质并映射到内存
    virtual void Draw();
protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[10] = {0};

};


#endif //XPLAY_XSHADER_H
