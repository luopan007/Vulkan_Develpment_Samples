#include "LightManager.h"
#include  <math.h>

float LightManager::lx=0;//初始化光源位置X
float LightManager::ly=0;//初始化光源位置Y
float LightManager::lz=0;//初始化光源位置Z
float LightManager::lightDiffuseR=0;//初始化散射光强度R分量
float LightManager::lightDiffuseG=0;//初始化散射光强度G分量
float LightManager::lightDiffuseB=0;//初始化散射光强度B分量
float LightManager::lightDiffuseA=0;//初始化散射光强度A分量
void LightManager::setLightPosition(float lxIn,float lyIn,float lzIn){ //设置光源位置的方法
    lx=lxIn;//设置光源位置X坐标
    ly=lyIn;//设置光源位置Y坐标
    lz=lzIn;//设置光源位置Z坐标
}
void LightManager::setlightDiffuse(float lightDiffuseRIn, float lightDiffuseGIn,float lightDiffuseBIn,float lightDiffuseAIn){//设置散射光强度的方法
    lightDiffuseR=lightDiffuseRIn;//设置散射光强度R分量
    lightDiffuseG=lightDiffuseGIn;//设置散射光强度G分量
    lightDiffuseB=lightDiffuseBIn;//设置散射光强度B分量
    lightDiffuseA=lightDiffuseAIn;//设置散射光强度A分量
}

