#ifndef VULKANEXBASE_TANSHE_H
#define VULKANEXBASE_TANSHE_H

#include <android/log.h>
#include <string>
#include "../util/ObjObject.h"
#include "../util/FileUtil.h"
#include "../bndev/MyVulkanManager.h"
using namespace std;//指定使用的命名空间
class TanShe {
public:
    static float tanshe_time ;         //弹射time
    static bool tanshe_flag ;         //弹射标志位
    static void TanSheCheck();    //检测是否弹射
};

#endif //VULKANEXBASE_TANSHE_H


