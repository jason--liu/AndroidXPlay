//
// Created by jason on 19-10-28.
//

#include "XShader.h"
#include "XLog.h"
#include <GLES2/gl2.h>
//顶点着色器glsl
#define GET_STR(x) #x //将传入的x直接转换为字符串且加了引号，比较清晰
static const char *vertexShader = GET_STR(
        attribute
        vec4 aPosition;//顶点坐标？
        attribute
        vec2 aTexCoord;//材质顶点坐标
        varying
        vec2 vTexCoord;//输出材质坐标,输出给片元着色器
        void main() {
            vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);//转换成LCD显示坐标，即原点在左上角
            gl_Position = aPosition;
        }
);//参考ijkplay

// 片元着色器
// p表示平面存储，即Y存完了再存U,V ffmpeg软解码和部分x86硬解码出来的格式
static const char *fragYUV420p = GET_STR(
        precision
        mediump float;//精度
        varying
        vec2 vTexCoord;//顶点着色器传递的坐标
        // 三个输入参数，输入材质（灰度材质，单像素）
        uniform
        sampler2D yTexture;
        uniform
        sampler2D uTexture;
        uniform
        sampler2D vTexture;
        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture, vTexCoord).r;
            yuv.g = texture2D(uTexture, vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture, vTexCoord).r - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

static const char *fragNV12 = GET_STR(
        precision
        mediump float;//精度
        varying
        vec2 vTexCoord;//顶点着色器传递的坐标
        // 三个输入参数，输入材质（灰度材质，单像素）
        uniform
        sampler2D yTexture;
        uniform
        sampler2D uvTexture;
        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture, vTexCoord).r;
            yuv.g = texture2D(uvTexture, vTexCoord).r - 0.5;
            yuv.b = texture2D(uvTexture, vTexCoord).a - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

static const char *fragNV21 = GET_STR(
        precision
        mediump float;//精度
        varying
        vec2 vTexCoord;//顶点着色器传递的坐标
        // 三个输入参数，输入材质（灰度材质，单像素）
        uniform
        sampler2D yTexture;
        uniform
        sampler2D uvTexture;
        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture, vTexCoord).r;
            yuv.g = texture2D(uvTexture, vTexCoord).a - 0.5;
            yuv.b = texture2D(uvTexture, vTexCoord).r - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

static GLuint InitShader(const char *code, GLint type) {
    // 创建shader
    GLuint sh = glCreateShader(type);
    if (!sh) {
        XLOGE("glCreateShader faild %d", type);
        return 0;
    }
    // 加载shader
    glShaderSource(sh,
                   1,//shader数量
                   &code, //shader执行代码
                   0);//第4个参数表示代码长度，0表示直接找字符串结尾
    //编译shader
    glCompileShader(sh);
    //获取编译情况
    GLint status;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if (!status) {
        XLOGE("glGetShaderiv failed type 0x%04x", type);
        return 0;
    }
    return sh;
}

bool XShader::Init(XShaderType type) {
    // shader初始化
    // 顶点shader初始化
    vsh = InitShader(vertexShader, GL_VERTEX_SHADER);
    if (!vsh) {
        XLOGE("InitShader vertexShader  failed ");
        return false;
    }
    XLOGD("InitShader vertexShader success!");
    // 片元yuv420p shader初始化
    switch (type) {
        case XSHADER_YUV420P:
            fsh = InitShader(fragYUV420p, GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV12:
            fsh = InitShader(fragNV12, GL_FRAGMENT_SHADER);
            break;
        case XSHADER_NV21:
            fsh = InitShader(fragNV21, GL_FRAGMENT_SHADER);
            break;
        default:
            XLOGE("xshader format error");
            return false;
    }

    if (!fsh) {
        XLOGE("InitShader fragYUV420p failed");
        return false;
    }
    XLOGD("InitShader fragYUV420p success!");

    // 创建渲染程序
    program = glCreateProgram();
    if (!program) {
        XLOGE("glCreateProgram failed");
        return false;
    }
    // 到这里就表示程序开始正常运行了
    // 渲染程序中加入着色器代码
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);

    // 链接程序
    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        XLOGE("glLink failed");
        return false;
    }
    XLOGD("glLink success");
    // 激活渲染程序
    glUseProgram(program);
    ////////////////////////////////////////////

    // 加入三维顶点数据 由两个三角形组成正方形
    static float vers[] = {
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
    };
    GLuint apos = glGetAttribLocation(program, "aPosition");//返回值要转换？
    glEnableVertexAttribArray(apos);
    // 传递顶点坐标
    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 12,
                          vers);//3表示一个点有xyz三个元素，12表示点存储间隔，3个浮点数占3x4=12字节
    // 加入材质坐标数据
    static float txts[] = {
            1.0f, 0.0f,//右下
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
    };
    GLuint atex = glGetAttribLocation(program, "aTexCoord");
    glEnableVertexAttribArray(atex);
    glVertexAttribPointer(atex, 2, GL_FLOAT, GL_FALSE, 8, txts);

    // 材质纹理初始化
    // 设置纹理层 将shader和yuv材质绑定？
    glUniform1i(glGetUniformLocation(program, "yTexture"), 0); //对应材质第一层
    switch (type) {
        case XSHADER_YUV420P:
            glUniform1i(glGetUniformLocation(program, "uTexture"), 1); //对应材质第二层
            glUniform1i(glGetUniformLocation(program, "vTexture"), 2); //对应材质第三层
            break;
        case XSHADER_NV12:
        case XSHADER_NV21:
            glUniform1i(glGetUniformLocation(program, "uvTexture"), 1);//对应材质第二层
    }


    XLOGD("shader init success");
    return true;
}

void XShader::GetTexture(unsigned int index, int width, int height, unsigned char *buf, bool isa) {
    unsigned int format = GL_LUMINANCE;
    if (isa)
        format = GL_LUMINANCE_ALPHA;
    if (texts[index] == 0) {
        //材质如果没有初始化过，初始化
        glGenTextures(1, &texts[index]);// 一次初始化一个
        // 设置纹理属性0
        glBindTexture(GL_TEXTURE_2D, texts[index]);
        // 缩小、放大的过滤器 因为视频可能拉伸放大
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//这个可以理解为设置渲染方法？
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 设置纹理格式和大小
        glTexImage2D(GL_TEXTURE_2D, 0,//细节基本 0默认
                     format,//gpu内部格式 亮度，灰度图
                     width, height, //尺寸是2的次方 拉伸到全屏
                     0,//边框
                     format,//数据格式，亮度
                     GL_UNSIGNED_BYTE,// 像素数据类型
                     NULL // 纹理数据，解码后再设置
        );
    }

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texts[index]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format,/*灰度图*/
                    GL_UNSIGNED_BYTE/*存储格式*/, buf);
}

void XShader::Draw() {
    if (!program)
        return;
    // 三维绘制
    // 这个函数会去读上面设置的顶点坐标信息，因此上面设置成static,
    // 这样才不会消失
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}