//
// Created by Administrator on 2018/5/9.
//

#ifndef VULKANEXBASE_SQUARE_PlaySound_H
#define VULKANEXBASE_SQUARE_PlaySound_H
#include <string>
#include <vector>

using namespace std;
class PlaySound {

public:

    static bool createEngine();//创建引擎
    static bool createHunYinQi();//创建混音器
    static bool createPlayer();//创建播放器
    static bool createPlayer_one(int index);

    static void playSound(const string fname);//播放某个音乐
    static void playBGM(const string fname);//循环播放背景音乐
    static void destroyObject();
    static void shutDown(const string fname);

    static std::vector<string> musicList;//音乐列表


};


#endif
