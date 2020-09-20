#include "LightManager.h"
#include  <math.h>

float LightManager::lightAmbientR=0;//初始化环境光强度R分量
float LightManager::lightAmbientG=0;//初始化环境光强度G分量
float LightManager::lightAmbientB=0;//初始化环境光强度B分量
float LightManager::lightAmbientA=0;//初始化环境光强度A分量
void LightManager::setlightAmbient(float lightAmbientRIn,float lightAmbientGIn,float lightAmbientBIn,float lightAmbientAIn){//设置环境光强度的方法
    lightAmbientR=lightAmbientRIn;//设置环境光强度R分量
    lightAmbientG=lightAmbientGIn;//设置环境光强度G分量
    lightAmbientB=lightAmbientBIn;//设置环境光强度B分量
    lightAmbientA=lightAmbientAIn;//设置环境光强度A分量
}

