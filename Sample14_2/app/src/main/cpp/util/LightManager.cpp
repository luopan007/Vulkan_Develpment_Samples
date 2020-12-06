#include "LightManager.h"
#include "MyVulkanManager.h"
#include "HelpFunction.h"
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

float lightDis=100;
void LightManager::setLightPosition()
{

//    lx=lxIn;
//    ly=lyIn;
//    lz=lzIn;

    double angradElevation =toRadians(MyVulkanManager::lightElevation);// 仰角（弧度）
    double angradAzimuth = toRadians(MyVulkanManager::lightAzimuth);// 方位角
    lx = (float) (- lightDis * cos(angradElevation)	* sin(angradAzimuth));
    ly = (float) (+ lightDis * sin(angradElevation));
    lz = (float) (- lightDis * cos(angradElevation) * cos(angradAzimuth));
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
