#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include "FFDemux.h"
#include "XLog.h"
#include "IDecode.h"
#include "FFDecode.h"
#include "XEGL.h"
#include "XShader.h"
#include "IVideoView.h"
#include "GLVideoView.h"
#include "FFResample.h"
#include "IAudioPlay.h"
#include "SLAudioPlay.h"

class TestObs : public IObserver {
public:
    void Update(XData d) {
        //XLOGI("test  obs update size %d",d.size);
    }
};

IVideoView *view = nullptr;
extern "C" JNIEXPORT jstring JNICALL
Java_com_jason_xplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    //IDemux *de = new FFDemux();
    //de->Open("/storage/emulated/0/impossible.mp4");///storage/emulated/0/1.pcm
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jason_xplay_MainActivity_testXplay(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    // TODO
    //Just For Test///
    IDemux *de = new FFDemux();
    TestObs *tobs = new TestObs();

    //de->AddObs(tobs);
    de->Open(url);///storage/emulated/0/1.pcm
    IDecode *vdecode = new FFDecode();
    vdecode->Open(de->GetVPara());

    IDecode *adecode = new FFDecode();
    adecode->Open(de->GetAPara());
    /* for(;;){
         XData d = de->Read();
         XLOGI("Read data size is %d",d.size);
     }*/
    de->AddObs(vdecode);
    de->AddObs(adecode);

    view = new GLVideoView();
    vdecode->AddObs(view);

    IResample *resample = new FFResample();
    XParameter outPara=de->GetAPara();//对象直接赋值
    resample->Open(de->GetAPara(), outPara);
    adecode->AddObs(resample);
    IAudioPlay *audioPlay = new SLAudioPlay();
    audioPlay->StartPlay(outPara);
    resample->AddObs(audioPlay);

    de->Start();
    vdecode->Start();
    adecode->Start();
    //XSleep(3000);
    //de->Stop();
    //env->ReleaseStringUTFChars(url_, url);
}extern "C"
JNIEXPORT void JNICALL
Java_com_jason_xplay_XPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {

    ANativeWindow* win =  ANativeWindow_fromSurface(env, surface);
    //XEGL::Get()->Init(win);
    //XShader shader;
    //shader.Init();
    view->SetRender(win);

}