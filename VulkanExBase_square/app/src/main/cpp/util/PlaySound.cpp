    //
    // Created by Administrator on 2018/7/12.
    //
    #include "PlaySound.h"
    #include "FileUtil.h"
    #include <SLES/OpenSLES.h>
    #include <SLES/OpenSLES_Android.h>
    #include <vector>
    #include <assert.h>
    #include <mylog.h>

    // 引擎接口
    SLObjectItf engineObject = NULL;//用SLObjectItf声明引擎接口对象
    SLEngineItf engineEngine = NULL;//声明具体的引擎对象实例

    //混音器
    SLObjectItf outputMixObject =NULL ;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    //assets播放器
    vector<SLObjectItf> fdPlayerObject;
    vector<SLPlayItf> fdPlayerPlay ;
    vector<SLVolumeItf> fdPlayerVolume ; //声音控制接口

    //缓冲器队列接口
    SLAndroidSimpleBufferQueueItf pcmBufferQueue;
    // pointer and size of the next player buffer to enqueue, and number of remaining buffers
    static short *nextBuffer;
    static unsigned nextSize;
    static int nextCount;
    //音乐列表
    std::vector<string>PlaySound::musicList={"sound/bgm.wav","sound/dingdong.wav"};

    bool PlaySound::createEngine(){//创建播放引擎
        slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);//第一步创建引擎
        (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);//实现（Realize）engineObject接口对象
        (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);//通过engineObject的GetInterface方法初始化engineEngine
        return true;
    }

    bool PlaySound::createHunYinQi() {//创建混音器
        createEngine();//调用方法，创建引擎
        /*================================创建混音器===============================*/
        SLresult result;

        for(int i=0;i<musicList.size();i++) {
            const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
            const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
            result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
            (void)result;
            result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
            (void)result;
            result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB, &outputMixEnvironmentalReverb);
            if (SL_RESULT_SUCCESS == result) {
                result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(outputMixEnvironmentalReverb, &reverbSettings);
                (void)result;
            }
        }
        return true;
    }

    bool PlaySound::createPlayer(){

        createHunYinQi();
        fdPlayerObject.resize(musicList.size());
        fdPlayerPlay.resize(musicList.size());
        fdPlayerVolume.resize(musicList.size());
        for(int i=0;i<musicList.size();i++) {
            createPlayer_one(i);
        }
        return true;
    }

    bool PlaySound::createPlayer_one(int index){
        AAsset* asset =AAssetManager_open(FileUtil::aam,musicList[index].c_str(),AASSET_MODE_UNKNOWN);
        off_t start, length;
        int fd = AAsset_openFileDescriptor(asset, &start, &length);
        AAsset_close(asset);

        SLresult result;


    /*==================================设置播放器参数和创建播放器===================================*/
    // 1、 配置 audio source
        SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
        SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
        SLDataSource audioSrc = {&loc_fd, &format_mime};

    // 2、 配置 audio sink
        SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
        SLDataSink audioSnk = {&loc_outmix, NULL};

        // 创建播放器
        const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
        const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
        result = (*engineEngine)->CreateAudioPlayer(engineEngine, &fdPlayerObject[index], &audioSrc, &audioSnk, 3, ids, req);
        (void)result;

        // 实现播放器
        result = (*fdPlayerObject[index])->Realize(fdPlayerObject[index], SL_BOOLEAN_FALSE);
        (void)result;

        // 得到播放器接口
        result = (*fdPlayerObject[index])->GetInterface(fdPlayerObject[index], SL_IID_PLAY, &fdPlayerPlay[index]);
        (void)result;

        // 得到声音控制接口
        result = (*fdPlayerObject[index])->GetInterface(fdPlayerObject[index], SL_IID_VOLUME, &fdPlayerVolume[index]);
        (void)result;
        return true;
    }

    int getMusicIndex(const string fname){
        int index=-1;
        for(int i=0;i<PlaySound::musicList.size();i++){
            if(PlaySound::musicList[i].compare(fname.c_str())==0)
            {
                index=i;
                break;
            }
        }
        assert(index!=-1);
        return index;
    }

    void PlaySound::playSound(const string fname){
        //设置播放状态
        int index=getMusicIndex(fname);
        SLPlayItf fdPlayerPlayTMP=NULL;
        fdPlayerPlayTMP=fdPlayerPlay[index];
        (*fdPlayerObject[index])->Destroy(fdPlayerObject[index]);
        fdPlayerObject[index]=NULL;
        if (NULL != fdPlayerPlayTMP) {
            createPlayer_one(index);
            fdPlayerPlayTMP=fdPlayerPlay[index];
            (*fdPlayerPlayTMP)->SetPlayState(fdPlayerPlayTMP, SL_PLAYSTATE_PLAYING);
            (*fdPlayerVolume[index])->SetVolumeLevel(fdPlayerVolume[index], 20 * -50);
        }
    }

    void PlaySound::playBGM(const string fname){
        //设置播放状态
        int index=getMusicIndex(fname);
        SLPlayItf fdPlayerPlayTMP=NULL;
        SLSeekItf fdPlayerSeek;
        fdPlayerPlayTMP=fdPlayerPlay[index];

        (*fdPlayerObject[index])->GetInterface(fdPlayerObject[index], SL_IID_SEEK, &fdPlayerSeek);//定位
        (*fdPlayerSeek)->SetLoop(fdPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);// 开启重复播放
        if (NULL != fdPlayerPlayTMP) {

            (*fdPlayerPlayTMP)->SetPlayState(fdPlayerPlayTMP, SL_PLAYSTATE_PLAYING);
        }
        //设置播放音量 （100 * -50：静音 ）
        (*fdPlayerVolume[index])->SetVolumeLevel(fdPlayerVolume[index], 20 * -50);
    }

    void PlaySound::shutDown(const string fname)
    {
        int index=getMusicIndex(fname);
        (*fdPlayerVolume[index])->SetVolumeLevel(fdPlayerVolume[index], 100 * -50);
    }

    void PlaySound::destroyObject()
    {
        for(int i=0;i<musicList.size();i++)
        {
            (*fdPlayerObject[i])->Destroy(fdPlayerObject[i]);
            fdPlayerObject[i]=NULL;
            fdPlayerPlay[i]=NULL;
            fdPlayerVolume [i]=NULL; //声音控制接口
        }
    }

