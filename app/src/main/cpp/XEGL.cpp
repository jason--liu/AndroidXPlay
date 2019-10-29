//
// Created by jason on 19-10-28.
//

#include "XEGL.h"
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include "XLog.h"

class CXEGL : public XEGL {
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;

    virtual bool Init(void *win) {
        ANativeWindow *nwin = static_cast<ANativeWindow *>(win);
        // 初始化EGL
        // 1.获取EGLDisplay对象
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            XLOGE("eglGetDisplay failed");
            return false;
        }
        XLOGD("eglGetDisplay success!");
        // 2.初始化display
        if (EGL_TRUE != eglInitialize(display, 0, 0)) {
            XLOGE("eglInitialize false!");
            return false;
        }
        XLOGD("eglInitialize success!");
        // 3.获取配置并创建Surface
        EGLint configSpec[] = {
                EGL_RED_SIZE, 8,// 红色8bit
                EGL_GREEN_SIZE, 8, //绿色8bit
                EGL_BLUE_SIZE, 8, //蓝色8bit
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,//窗口类型
                EGL_NONE
        };
        EGLConfig config = 0;
        EGLint numConfig = 0;
        if (EGL_TRUE !=
            eglChooseConfig(display, configSpec, &config, 1/*想获得1个config*/, &numConfig/*获取返回数*/)) {
            XLOGE("eglChooseConfig false!");
            return false;

        }
        XLOGD("eglChooseConfig success!");

        // 4.创建Surface
        surface = eglCreateWindowSurface(display, config, nwin, nullptr);

        // 5.创建并打开EGL上下文
        const EGLint ctxAttr[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
        };
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
        if (context == EGL_NO_CONTEXT) {
            XLOGE("eglCreateContext false!");
            return false;
        }
        XLOGD("eglCreateContext success!");

        // 6.上下文切换
        if (EGL_TRUE != eglMakeCurrent(display, surface, surface, context)) {
            XLOGE("eglMakeCurrent false!");
            return false;
        }
        XLOGD("eglMakeCurrent success!");
        return true;
    }

    virtual void Draw() {
        if (display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE)
            return;
        eglSwapBuffers(display, surface);
    }
};

XEGL *XEGL::Get() {
    static CXEGL egl;
    return &egl;
}