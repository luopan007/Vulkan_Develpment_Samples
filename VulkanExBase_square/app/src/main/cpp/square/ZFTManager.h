#ifndef VULKANEXBASE_ZFTMANAGER_H
#define VULKANEXBASE_ZFTMANAGER_H

#include "Square3D.h"
class ZFTManager {

public:

    static bool life ;                   //存活
    static int Diefoncution;
    static int Box_pos ;                   //方块所在的位置第几格
    static float MainScalex;
    static float MainScaley;
    static float MainScalez;
    static void Update_mainbox();
    static void ZFTDiCiDie();						//正方体地刺死亡动画
    static void ZFTJiaZiDie();				//正方体夹子死亡动画
    static void ZFTDiaoLuoDie();            //掉落刺穿方块死亡动画
    static void ZFTYiDongDie();            //移动方块死亡动画
    static void Win();            //移动方块死亡动画
};

#endif //VULKANEXBASE_ZFTMANAGER_H
