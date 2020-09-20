#include "LightManager.h"
#include  <math.h>
#include <mylog.h>

float LightManager::lx=0; //定向光方向向量X 分量
float LightManager::ly=0; //定向光方向向量Y 分量
float LightManager::lz=0; //定向光方向向量Z 分量
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

void LightManager::setLightDirection(float lxIn, float lyIn,float lzIn){ //设置定向光方向的方法
    lx=lxIn; //设置定向光方向向量X 分量
    ly=lyIn; //设置定向光方向向量Y 分量
    lz=lzIn; //设置定向光方向向量Z 分量
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
