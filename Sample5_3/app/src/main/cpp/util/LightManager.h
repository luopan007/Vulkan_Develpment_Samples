#ifndef VULKANEXBASE_LIGHTMANAGER_H
#define VULKANEXBASE_LIGHTMANAGER_H//防止重复引用
class LightManager{
public:
    static float lx,ly,lz;	//光源位置
    static float lightDiffuseR,lightDiffuseG,lightDiffuseB,lightDiffuseA;//散射光强度RGBA分量
    static void setLightPosition(float lxIn,float lyIn,float lzIn);//设置光源位置的方法
    static void setlightDiffuse(float lightDiffuseRIn,float lightDiffuseGIn,float lightDiffuseBIn,float lightDiffuseAIn);//设置散射光强度RGBA分量
};
#endif
