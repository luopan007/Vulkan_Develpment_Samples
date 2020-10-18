#include <mylog.h>
#include "YiDongBox.h"
#include "../util/LoadUtil.h"
#include "../bndev/MyVulkanManager.h"
#include "ZFTManager.h"
#include "My3DLayer.h"

int YiDongBox::yidong_time = 0;
bool YiDongBox::yidong_up = false;
int YiDongBox::YiDongIndex = 0;
int countRL=0;//判断左右
void YiDongBox::YiDongTimeUpdate(){
    YiDongBox::yidong_time ++;
}
float YiDongBox::YiDongUpDown(){
    if(countRL==0){
        if (YiDongBox::yidong_time < 20){
            YiDongBox::yidong_up = true;//true
            return yidong_time * 5.0f;
        }else if(YiDongBox::yidong_time < 50){
            return 20 * 5.0f;
        }else if(YiDongBox::yidong_time < 70){
            YiDongBox::yidong_up = false;
            return 20 * 5.0f - (yidong_time-50) * 5.0f;
        }else if(YiDongBox::yidong_time < 120){
            return 0.0f;
        }else{
            YiDongBox::yidong_time = 0;
            countRL=1;
            return 0.0f;
        }
    }
    else{
        if (YiDongBox::yidong_time < 20){
            YiDongBox::yidong_up = true;
            return -(yidong_time * 5.0f);
        }else if(YiDongBox::yidong_time < 50){
            return -(20 * 5.0f);
        }else if(YiDongBox::yidong_time < 70){
            YiDongBox::yidong_up = false;//true
            return -(20 * 5.0f - (yidong_time-50) * 5.0f);
        }else if(YiDongBox::yidong_time < 120){
            return 0.0f;
        }else{
            YiDongBox::yidong_time = 0;
            countRL=0;
            return 0.0f;
        }
    }

}
void YiDongBox::YiDongCheckLife(){
    //主角到地刺上
    if(ZFTManager::Box_pos == My3DLayer::yidongpos[YiDongBox::YiDongIndex]){
        if(YiDongBox::yidong_up){
            ZFTManager::life = false;
            ZFTManager::Diefoncution=4;
        }
    }
    if(ZFTManager::Box_pos > My3DLayer::dicipos[YiDongBox::YiDongIndex]&&YiDongBox::YiDongIndex<= YIDONG_NUMBER)
        YiDongBox::YiDongIndex++;
}