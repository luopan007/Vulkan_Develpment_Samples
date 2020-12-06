#include "LightManager.h"
#include "MyVulkanManager.h"
#include "HelpFunction.h"
#include "MatrixState3D.h"
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
float LightManager::lcm[16];
float LightManager::lpm[16];
float LightManager::lcpm[16];
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
void LightManager::getTYJZ(float ratio)
{
    Matrix::frustumM(lpm,0, -ratio, ratio, -1.0f, 1.0f, 0.75f, 400);
    Matrix::setLookAtM(lcm, 0,40,60,0,0,0,0,-3,2,0);
    Matrix::multiplyMM(lcpm, 0, lpm, 0, lcm, 0);
    Matrix::multiplyMM(lcpm, 0, MatrixState3D::vulkanClipMatrix, 0, lcpm, 0);
}
