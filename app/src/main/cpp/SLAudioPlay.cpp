//
// Created by jason on 19-11-3.
//

#include "SLAudioPlay.h"
#include "XLog.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#define ASSERT_EQ(x, y) do { if ((x) == (y)) ; else { XLOGE( "0x%x != 0x%x\n", \
    (unsigned) (x), (unsigned) (y)); assert((x) == (y)); } } while (0)

static SLObjectItf engineObject = nullptr;
static SLEngineItf engineEngine = nullptr;
static SLObjectItf outputmixObject = nullptr;
static SLObjectItf playerObject = nullptr;
static SLPlayItf playerPlay = nullptr;
SLAndroidSimpleBufferQueueItf playerBufferQueue = nullptr;

static SLEngineItf CreateSL() {
    SLresult result;
    SLEngineItf en;
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    ASSERT_EQ(SL_RESULT_SUCCESS, result);
    // SL_BOOLEAN_FALSE阻塞式创建
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    ASSERT_EQ(SL_RESULT_SUCCESS, result);
    SLEngineItf engineEngine;
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    ASSERT_EQ(SL_RESULT_SUCCESS, result);
    return engineEngine;

}

static void playerCallback(SLAndroidSimpleBufferQueueItf buf, void *pContext) {
    SLAudioPlay *ap = static_cast<SLAudioPlay *>(pContext);
    ap->PlayCall((void *) (buf));
}

void SLAudioPlay::PlayCall(void *bufq) {
    if (!bufq)
        return;
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf) (bufq);
    XLOGD("SLAudioPlay::PlayCall called");
    return;
}

bool SLAudioPlay::StartPlay(XParameter out) {
    SLresult result;
    //1.创建引擎
    engineEngine = CreateSL();
    if (!engineEngine) {
        XLOGE("createSL fail");
        return false;
    }
    XLOGD("createSL success");

    // 2.创建输出混音器
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputmixObject, 0, NULL, NULL);
    ASSERT_EQ(SL_RESULT_SUCCESS, result);
    result = (*outputmixObject)->Realize(outputmixObject, SL_BOOLEAN_FALSE);
    ASSERT_EQ(SL_RESULT_SUCCESS, result);

    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX, outputmixObject};
    SLDataSink audioSink = {&outmix, 0};

    // 3.配置音频信息
    // 缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    // 音频格式配置
    SLDataFormat_PCM pcm = {
            .formatType=SL_DATAFORMAT_PCM,
            .numChannels = (SLuint32) out.channels,
            .samplesPerSec = (SLuint32) (out.sample_rate * 1000),// SL_SAMPLINGRATE_44_1,
            .bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16,
            .containerSize = SL_PCMSAMPLEFORMAT_FIXED_16,
            .channelMask=SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            .endianness = SL_BYTEORDER_LITTLEENDIAN
    };

    SLDataSource ds = {&que, &pcm};

    // 4.创建播放器
    // 表示需要这个接口，待会下面的GetInterface才能获取到
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    // 表示接口开放还是关闭，TRUE表示开放
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &ds, &audioSink,
                                                sizeof(ids) / sizeof(SLInterfaceID)/*表示参数个数*/, ids,
                                                req);
    ASSERT_EQ(SL_RESULT_SUCCESS, result);
    XLOGD("create AudioPlayer success");

    result = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
    ASSERT_EQ(SL_RESULT_SUCCESS, result);
    // 获取播放接口
    result = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerPlay);
    ASSERT_EQ(SL_RESULT_SUCCESS, result);
    // 获取队列接口
    result = (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, &playerBufferQueue);
    ASSERT_EQ(SL_RESULT_SUCCESS, result);
    // 注册回调函数
    result = (*playerBufferQueue)->RegisterCallback(playerBufferQueue, playerCallback,
                                                    this);
    ASSERT_EQ(SL_RESULT_SUCCESS, result);
    // 设置播放状态
    (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
    // 启动回调队列
    (*playerBufferQueue)->Enqueue(playerBufferQueue, "", 1);
    XLOGD("SLAudioPlay: StartPlay success");
    return true;
}
