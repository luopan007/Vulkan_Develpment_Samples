#include "LightManager.h"
#include  <math.h>

//光源位置
float LightManager::lx=0;
float LightManager::ly=0;
float LightManager::lz=0;
float LightManager::lightSpecularR=0; //初始化镜面光强度R 分量
float LightManager::lightSpecularG=0; //初始化镜面光强度G 分量
float LightManager::lightSpecularB=0; //初始化镜面光强度B 分量
float LightManager::lightSpecularA=0; //初始化镜面光强度A 分量

//设置光源的位置
void LightManager::setLightPosition(float lxIn,float lyIn,float lzIn)
{
    lx=lxIn;
    ly=lyIn;
    lz=lzIn;
}
void LightManager::setlightSpecular(float lightSpecularRIn,float lightSpecularGIn,float lightSpecularBIn,float lightSpecularAIn){ //设置镜面光强度的方法
    lightSpecularR=lightSpecularRIn; //设置镜面光强度R 分量
    lightSpecularG=lightSpecularGIn; //设置镜面光强度G 分量
    lightSpecularB=lightSpecularBIn; //设置镜面光强度B 分量
    lightSpecularA=lightSpecularAIn; //设置镜面光强度A 分量
}
