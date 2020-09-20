#ifndef VULKANEXBASE_LIGHTMANAGER_H
#define VULKANEXBASE_LIGHTMANAGER_H
class LightManager
{
public:
    static float lx,ly,lz; //光源方向向量的XYZ 分量
    static void setLightDirection(float lxIn,float lyIn,float lzIn); //设置光源方向的方法
    static float lightAmbientR,lightAmbientG,lightAmbientB,lightAmbientA;
    static float lightDiffuseR,lightDiffuseG,lightDiffuseB,lightDiffuseA;
    static float lightSpecularR,lightSpecularG,lightSpecularB,lightSpecularA;
    static void setlightAmbient(float lightAmbientRIn,float lightAmbientGIn,float lightAmbientBIn,float lightAmbientAIn);
    static void setlightDiffuse(float lightDiffuseRIn,float lightDiffuseGIn,float lightDiffuseBIn,float lightDiffuseAIn);
    static void setlightSpecular(float lightSpecularRIn,float lightSpecularGIn,float lightSpecularBIn,float lightSpecularAIn);
};


#endif
