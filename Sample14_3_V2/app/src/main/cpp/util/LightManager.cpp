#include "LightManager.h"
#include  <math.h>
#include "Matrix.h"
#include "MatrixState3D.h"
#define LIGHT_R 360;
float LightManager::fwj=0;
float LightManager::lx=0;
float LightManager::ly=0;
float LightManager::lz=0;
float LightManager::upxYS;
float LightManager::upyYS;
float LightManager::upzYS;
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
void LightManager::setLightPosition(float fwjSpan,float yj)
{
    fwj=fwj+fwjSpan;
    if(fwj>360)
    {
        fwj=fwj-360;
    }
    ly=sin(yj*3.1415926535898/180)*LIGHT_R;
    float lxz=cos(yj*3.1415926535898/180)*LIGHT_R;
    lx=sin(fwj*3.1415926535898/180)*lxz;
    lz=cos(fwj*3.1415926535898/180)*lxz;
    upyYS=cos(yj*3.1415926535898/180);
    float upXZ=sin(yj*3.1415926535898/180);
    upxYS=-upXZ*sin(fwj*3.1415926535898/180);
    upzYS=-upXZ*cos(fwj*3.1415926535898/180);
}
float* normalize(float x,float y,float z)
{
    float* result=new float[3];
    float length=sqrt(x*x+y*y+z*z);
    result[0]=x/length;
    result[1]=y/length;
    result[2]=z/length;
    return result;
}
void LightManager::getTYJZ(float ratio)
{
    Matrix::frustumM(lpm,0, -ratio, ratio, -1.0f, 1.0f, LIGNT_CAMERA_NEAR, LIGNT_CAMERA_FAR);
    Matrix::setLookAtM(lcm, 0, lx, ly, lz, 0, 0, 0,upxYS,upyYS,upzYS);
    Matrix::multiplyMM(lcpm, 0, lpm, 0, lcm, 0);
    Matrix::multiplyMM(lcpm, 0, MatrixState3D::vulkanClipMatrix, 0, lcpm, 0);
}
