#include "LightManager.h"
#include  <math.h>
float LightManager::lx=0;
float LightManager::ly=0;
float LightManager::lz=0;
float LightManager::lightAmbientR=0;
float LightManager::lightAmbientG=0;
float LightManager::lightAmbientB=0;
float LightManager::lightAmbientA=0;
float LightManager::lightDiffuseR=0;
float LightManager::lightDiffuseG=0;
float LightManager::lightDiffuseB=0;
float LightManager::lightDiffuseA=0;
float LightManager::lightSpecularR=0;
float LightManager::lightSpecularG=0;
float LightManager::lightSpecularB=0;
float LightManager::lightSpecularA=0;
float LightManager::lightFWJ=0;
void LightManager::setLightPosition(float lxIn,float lyIn,float lzIn)
{
    lx=lxIn;
    ly=lyIn;
    lz=lzIn;
}
void LightManager::setlightAmbient(float lightAmbientRIn,float lightAmbientGIn,float lightAmbientBIn,float lightAmbientAIn)
{
    lightAmbientR=lightAmbientRIn;
    lightAmbientG=lightAmbientGIn;
    lightAmbientB=lightAmbientBIn;
    lightAmbientA=lightAmbientAIn;
}
void LightManager::setlightDiffuse(float lightDiffuseRIn,float lightDiffuseGIn,float lightDiffuseBIn,float lightDiffuseAIn)
{
    lightDiffuseR=lightDiffuseRIn;
    lightDiffuseG=lightDiffuseGIn;
    lightDiffuseB=lightDiffuseBIn;
    lightDiffuseA=lightDiffuseAIn;
}
void LightManager::setlightSpecular(float lightSpecularRIn,float lightSpecularGIn,float lightSpecularBIn,float lightSpecularAIn)
{
    lightSpecularR=lightSpecularRIn;
    lightSpecularG=lightSpecularGIn;
    lightSpecularB=lightSpecularBIn;
    lightSpecularA=lightSpecularAIn;
}
void LightManager::move(float fwjSpan){ //移动光源的方法
    lightFWJ=lightFWJ+fwjSpan; //光源方位角
    float tempLx=float(sin(lightFWJ/180*3.14150265)*300); //计算光源坐标 X 分量
    float tempLz=float(cos(lightFWJ/180*3.14150265)*300); //计算光源坐标 Z 分量
    LightManager::setLightPosition(tempLx,0,tempLz); //设置光源位置
    if(lightFWJ>=360){ //若光源方位角大于大于 360 度
        lightFWJ=0; //光源方位角归零
    }}


