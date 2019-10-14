#include <jni.h>
#include <string>
#include "FFDemux.h"
extern "C" JNIEXPORT jstring JNICALL
Java_com_jason_xplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    IDemux *de = new FFDemux();
    de->Open("/storage/0");///storage/emulated/0/1.pcm
    return env->NewStringUTF(hello.c_str());
}
