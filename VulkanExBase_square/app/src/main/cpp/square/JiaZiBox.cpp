#include <mylog.h>
#include "JiaZiBox.h"
#include "../bndev/MyVulkanManager.h"
#include "ZFTManager.h"
#include "My3DLayer.h"

int JiaZiBox::jiazi_time = 0;
bool JiaZiBox::jiazi_up = false;
int JiaZiBox::JiaZiIndex = 0;

void JiaZiBox::JiaZiTimeUpdate(){
    JiaZiBox::jiazi_time ++;
}

float JiaZiBox::JiaZiUpDown(){
    if (JiaZiBox::jiazi_time < 30){
        JiaZiBox::jiazi_up = false;
        return jiazi_time * 1.5f;
    }else if(JiaZiBox::jiazi_time < 60){
        return 30 * 1.5f;
    }else if(JiaZiBox::jiazi_time < 90){
        JiaZiBox::jiazi_up = true;//
        return 30 * 1.5f - (jiazi_time-60) * 1.5f;
    }else if(JiaZiBox::jiazi_time < 120){
        return 0.0f;
    }else{
        JiaZiBox::jiazi_time = 0;
        return 0.0f;
    }
}
void JiaZiBox::JiaZiCheckLife(){
    //主角到地刺上
    if(ZFTManager::Box_pos == My3DLayer::jiazipos[JiaZiBox::JiaZiIndex]){
        if(JiaZiBox::jiazi_up){
            ZFTManager::life = false;
            ZFTManager::Diefoncution=2;
        }
    }
    if(ZFTManager::Box_pos > My3DLayer::jiazipos[JiaZiBox::JiaZiIndex]&&JiaZiBox::JiaZiIndex<= JIAZI_NUMBER)
        JiaZiBox::JiaZiIndex++;
}