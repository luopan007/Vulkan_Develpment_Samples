#include <mylog.h>
#include "DiCiBox.h"
#include "../util/LoadUtil.h"
#include "../bndev/MyVulkanManager.h"
#include "ZFTManager.h"
#include "My3DLayer.h"

int DiCiBox::dici_time = 0;
bool DiCiBox::dici_up = false;
int DiCiBox::DiCiIndex = 0;

void DiCiBox::DiCiTimeUpdate(){
    DiCiBox::dici_time ++;
}
float DiCiBox::DiCiUpDown(){
    if (DiCiBox::dici_time < 30){
        DiCiBox::dici_up = true;//
        return dici_time * 1.5f;
    }else if(DiCiBox::dici_time < 60){
        return 30 * 1.5f;
    }else if(DiCiBox::dici_time < 90){
        DiCiBox::dici_up = false;
        return 30 * 1.5f - (dici_time-60) * 1.5f;
    }else if(DiCiBox::dici_time < 120){
        return 0.0f;
    }else{
        DiCiBox::dici_time = 0;
        return 0.0f;
    }
}
void DiCiBox::DiCiCheckLife(){
    //主角到地刺上
    if(ZFTManager::Box_pos == My3DLayer::dicipos[DiCiBox::DiCiIndex]){
        if(DiCiBox::dici_up){
            ZFTManager::life = false;
            ZFTManager::Diefoncution=1;
        }
    }
    if(ZFTManager::Box_pos > My3DLayer::dicipos[DiCiBox::DiCiIndex]&&DiCiBox::DiCiIndex<= DICI_NUMBER)
        DiCiBox::DiCiIndex++;
}