//
// Created by jason on 19-10-14.
//

#ifndef XPLAY_XLOG_H
#define XPLAY_XLOG_H


class XLog {

};

#ifdef  ANDROID
#include <android/log.h>
#define TAG "XPlay"
#define XLOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define XLOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define XLOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define XLOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#else

#endif
#endif //XPLAY_XLOG_H
