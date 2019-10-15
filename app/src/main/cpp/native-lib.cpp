#include <jni.h>
#include <string>
#include "FFDemux.h"
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
    IDemux *de = new FFDemux();
    de->Open(url);///storage/emulated/0/1.pcm
    env->ReleaseStringUTFChars(url_, url);
}