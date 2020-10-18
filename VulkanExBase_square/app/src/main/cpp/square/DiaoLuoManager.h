#ifndef VULKANEXBASE_DIAOLUOMANAGER_H
#define VULKANEXBASE_DIAOLUOMANAGER_H

#include <android/log.h>
#include <string>
#include "../util/ObjObject.h"
#include "../util/FileUtil.h"
#include "../bndev/MyVulkanManager.h"
using namespace std;//指定使用的命名空间
class DiaoLuoManager {
public:
    static int diaoluo_time;
    static bool diaoluo_up;    //地刺上升了
    static bool diaoluo_down;
    static int DiaoLuoIndex ;         //地刺格子个数
    static float DiaoLuoUpDown();//地刺升起与放下
    static void DiaoLuoTimeUpdate();//地刺升起下落
    static void DiaoLuoCheckLife();    //检测是否被地刺扎死
};


#endif //VULKANEXBASE_DIAOLUOMANAGER_H
