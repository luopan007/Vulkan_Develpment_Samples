#include "LightManager.h"
#include  <math.h>

//光源位置
float LightManager::lx=0;
float LightManager::ly=0;
float LightManager::lz=0;
//环境光强度RGBA
float LightManager::lightAmbientR=0;
float LightManager::lightAmbientG=0;
float LightManager::lightAmbientB=0;
float LightManager::lightAmbientA=0;
//散射光强度RGBA
float LightManager::lightDiffuseR=0;
float LightManager::lightDiffuseG=0;
float LightManager::lightDiffuseB=0;
float LightManager::lightDiffuseA=0;
//镜面光强度RGBA
float LightManager::lightSpecularR=0;
float LightManager::lightSpecularG=0;
float LightManager::lightSpecularB=0;
float LightManager::lightSpecularA=0;

//特定于具体案例使用
//光源位置方位角
float LightManager::lightFWJ=0;

//设置光源的位置
void LightManager::setLightPosition(float lxIn,float lyIn,float lzIn)
{
    lx=lxIn;
    ly=lyIn;
    lz=lzIn;
}
//设置环境光强度RGBA
void LightManager::setlightAmbient(float lightAmbientRIn,float lightAmbientGIn,float lightAmbientBIn,float lightAmbientAIn)
{
    lightAmbientR=lightAmbientRIn;
    lightAmbientG=lightAmbientGIn;
    lightAmbientB=lightAmbientBIn;
    lightAmbientA=lightAmbientAIn;
}
//设置散射光强度RGBA
void LightManager::setlightDiffuse(float lightDiffuseRIn,float lightDiffuseGIn,float lightDiffuseBIn,float lightDiffuseAIn)
{
    lightDiffuseR=lightDiffuseRIn;
    lightDiffuseG=lightDiffuseGIn;
    lightDiffuseB=lightDiffuseBIn;
    lightDiffuseA=lightDiffuseAIn;
}
//设置镜面光强度RGBA
void LightManager::setlightSpecular(float lightSpecularRIn,float lightSpecularGIn,float lightSpecularBIn,float lightSpecularAIn)
{
    lightSpecularR=lightSpecularRIn;
    lightSpecularG=lightSpecularGIn;
    lightSpecularB=lightSpecularBIn;
    lightSpecularA=lightSpecularAIn;
}

//特定于具体案例的光源位置移动方法
void LightManager::move(float fwjSpan)
{
    lightFWJ=lightFWJ+fwjSpan;
    float tempLx=sin(lightFWJ/180*3.14150265)*300;
    float tempLz=cos(lightFWJ/180*3.14150265)*300;
    LightManager::setLightPosition(tempLx,100,tempLz);
    if(lightFWJ>=360)
    {
        lightFWJ=0;
    }
}