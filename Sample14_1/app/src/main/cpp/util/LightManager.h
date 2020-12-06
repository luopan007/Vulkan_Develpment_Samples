//
// Created by Administrator on 2017/3/8 0008.
//

#ifndef VULKANEXBASE_LIGHTMANAGER_H
#define VULKANEXBASE_LIGHTMANAGER_H


class LightManager
{
public:
    //光源位置
    static float lx,ly,lz;
    //环境光强度RGBA
    static float lightAmbientR,lightAmbientG,lightAmbientB,lightAmbientA;
    //散射光强度RGBA
    static float lightDiffuseR,lightDiffuseG,lightDiffuseB,lightDiffuseA;
    //镜面光强度RGBA
    static float lightSpecularR,lightSpecularG,lightSpecularB,lightSpecularA;

    //设置光源的位置
    static void setLightPosition(float lxIn,float lyIn,float lzIn);
    //设置环境光强度RGBA
    static void setlightAmbient(float lightAmbientRIn,float lightAmbientGIn,float lightAmbientBIn,float lightAmbientAIn);
    //设置散射光强度RGBA
    static void setlightDiffuse(float lightDiffuseRIn,float lightDiffuseGIn,float lightDiffuseBIn,float lightDiffuseAIn);
    //设置镜面光强度RGBA
    static void setlightSpecular(float lightSpecularRIn,float lightSpecularGIn,float lightSpecularBIn,float lightSpecularAIn);

    //下面的为某案例特定代码，运动光源用======================================
    //服务于光源位置变化的参数(光源位置方位角)
    static float lightFWJ;
    static void move();

};


#endif //VULKANEXBASE_LIGHTMANAGER_H
