#include "TanShe.h"
#include <mylog.h>
#include "../util/LoadUtil.h"
#include "../bndev/MyVulkanManager.h"
#include "ZFTManager.h"
#include "My3DLayer.h"
#include "../util/CameraUtil.h"
float TanShe::tanshe_time = 0.0;
bool TanShe::tanshe_flag = false;

void TanShe::TanSheCheck(){
    //主方块到达弹射位置
    if(ZFTManager::Box_pos == My3DLayer::tanshepos[0]){
        tanshe_flag= true;
        ZFTManager::Box_pos+=4;
    }
    if(tanshe_flag){
        tanshe_time+=4;
        if(tanshe_time == 100.0){
            tanshe_flag = false;
            tanshe_time = 0.0;
            My3DLayer::mainBox->y=My3DLayer::sp3Tree[ZFTManager::Box_pos]->y + 40;
            My3DLayer::mainBox->x=My3DLayer::sp3Tree[ZFTManager::Box_pos]->x;
            My3DLayer::mainBox->z=My3DLayer::sp3Tree[ZFTManager::Box_pos]->z;
        }
        else {//移动1%

            My3DLayer::mainBox->x=(int)(My3DLayer::sp3Tree[ZFTManager::Box_pos - 4]->x + (My3DLayer::sp3Tree[ZFTManager::Box_pos]->x - My3DLayer::sp3Tree[ZFTManager::Box_pos - 4]->x) * (tanshe_time/100.0));
            My3DLayer::mainBox->z=(int)(My3DLayer::sp3Tree[ZFTManager::Box_pos - 4]->z + (My3DLayer::sp3Tree[ZFTManager::Box_pos]->z  - My3DLayer::sp3Tree[ZFTManager::Box_pos - 4]->z) * (tanshe_time/100.0));
            if(tanshe_time < 50.0){
                My3DLayer::mainBox->y=((My3DLayer::sp3Tree[ZFTManager::Box_pos]->y) + (int)(300 * (tanshe_time/100.0)));
            }else{
                My3DLayer::mainBox->y=((My3DLayer::sp3Tree[ZFTManager::Box_pos]->y) + (int)(300 * ((100.0-tanshe_time)/100.0)));
            }
        }
    }
}
