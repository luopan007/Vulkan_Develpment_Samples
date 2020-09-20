#ifndef VULKANEXBASE_LIGHTMANAGER_H
#define VULKANEXBASE_LIGHTMANAGER_H //防止重复引用
class LightManager{
public:
    static float lx,ly,lz; //光源位置XYZ 分量
    static float lightSpecularR,lightSpecularG,lightSpecularB,lightSpecularA; //镜面光强度RGBA 分量
    static void setLightPosition(float lxIn,float lyIn,float lzIn); //设置光源的位置的方法
    static void setlightSpecular(float lightSpecularRIn,float //设置镜面光强度的方法
    lightSpecularGIn,float lightSpecularBIn,float lightSpecularAIn);
};
#endif
