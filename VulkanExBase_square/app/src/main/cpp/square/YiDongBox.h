#ifndef VULKANEXBASE_YIDONGBOX_H
#define VULKANEXBASE_YIDONGBOX_H
#include <android/log.h>
#include <string>
#include "../util/ObjObject.h"
#include "../util/FileUtil.h"
#include "../bndev/MyVulkanManager.h"
using namespace std;//指定使用的命名空间
class YiDongBox {
public:
    static int yidong_time;
    static bool yidong_up;    //地刺上升了
    static int YiDongIndex ;         //地刺格子个数
    static float YiDongUpDown();//地刺升起与放下
    static void YiDongTimeUpdate();//地刺升起下落
    static void YiDongCheckLife();    //检测是否被地刺扎死
};


#endif //VULKANEXBASE_YIDONGBOX_H

