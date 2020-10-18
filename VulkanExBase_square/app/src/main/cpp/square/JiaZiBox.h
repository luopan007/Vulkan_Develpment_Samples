//
// Created by song on 2017/8/3.
//

#ifndef VULKANEXBASE_SQUARE_JIAZIBOX_H
#define VULKANEXBASE_SQUARE_JIAZIBOX_H


#include <android/log.h>
#include <string>
#include "../util/ObjObject.h"
#include "../util/FileUtil.h"
#include "../bndev/MyVulkanManager.h"
using namespace std;//指定使用的命名空间
class JiaZiBox
{
public:
    static int jiazi_time;
    static bool jiazi_up;    //地刺上升了
    static int JiaZiIndex ;         //地刺格子个数
    static void JiaZiTimeUpdate();
    static float JiaZiUpDown();//地刺升起与放下
    static void JiaZiCheckLife();    //检测是否被地刺扎死
};

#endif //VULKANEXBASE_SQUARE_JIAZIBOX_H
