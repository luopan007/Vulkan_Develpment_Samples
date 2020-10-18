#include <mylog.h>
#include "../util/LoadUtil.h"
#include "../bndev/MyVulkanManager.h"
#include "ZFTManager.h"
#include "My3DLayer.h"
#include "DiaoLuoManager.h"
int DiaoLuoManager::diaoluo_time = 0;
bool DiaoLuoManager::diaoluo_up = false;
bool DiaoLuoManager::diaoluo_down = false;
int DiaoLuoManager::DiaoLuoIndex = 0;

void DiaoLuoManager::DiaoLuoTimeUpdate(){
    DiaoLuoManager::diaoluo_time ++;
}
float DiaoLuoManager::DiaoLuoUpDown(){
    if (DiaoLuoManager::diaoluo_time <30){
        DiaoLuoManager::diaoluo_up = false;
        DiaoLuoManager::diaoluo_down = false;
        return diaoluo_time * 2.6f;
    }else if(DiaoLuoManager::diaoluo_time < 60){
        return 30 * 2.6f;
    }else if(DiaoLuoManager::diaoluo_time < 90){
        DiaoLuoManager::diaoluo_up = true;
        return 30 * 2.6f - (diaoluo_time-60) * 2.6f;
    }else if(DiaoLuoManager::diaoluo_time < 120){
        DiaoLuoManager::diaoluo_down = true;
        return 0.0f;
    }else{
        DiaoLuoManager::diaoluo_time = 0;
        return 0.0f;
    }
}
void DiaoLuoManager::DiaoLuoCheckLife() {
    //主角到地刺上
    if(ZFTManager::Box_pos == My3DLayer::diaoluopos[DiaoLuoManager::DiaoLuoIndex]){
        if(DiaoLuoManager::diaoluo_up){
            ZFTManager::life = false;
            ZFTManager::Diefoncution=3;
        }
    }
    if(ZFTManager::Box_pos > My3DLayer::diaoluopos[DiaoLuoManager::DiaoLuoIndex]&&DiaoLuoManager::DiaoLuoIndex<= DIAOLUO_NUMBER)
        DiaoLuoManager::DiaoLuoIndex++;
}