#ifndef VULKANEXBASE_LIGHTMANAGER_H
#define VULKANEXBASE_LIGHTMANAGER_H	//防止重复引用
class LightManager{
public:
    static float lightAmbientR,lightAmbientG,lightAmbientB,lightAmbientA;//环境光强度RGBA分量
    static void setlightAmbient(float lightAmbientRIn,float lightAmbientGIn,float lightAmbientBIn,float lightAmbientAIn);//设置环境光强度的方法
};
#endif
