#ifndef VULKANEXBASE_LIGHTMANAGER_H
#define VULKANEXBASE_LIGHTMANAGER_H
class LightManager
{
public:
    static float lx,ly,lz;
    static float lightAmbientR,lightAmbientG,lightAmbientB,lightAmbientA;
    static float lightDiffuseR,lightDiffuseG,lightDiffuseB,lightDiffuseA;
    static float lightSpecularR,lightSpecularG,lightSpecularB,lightSpecularA;
    static float lcm[16];
    static float lpm[16];
    static float lcpm[16];
    static float upxYS, upyYS, upzYS;
    static float upDegree;
    static void setLightPosition(float x, float y, float z);
    static void setlightAmbient(float lightAmbientRIn,float lightAmbientGIn,float lightAmbientBIn,float lightAmbientAIn);
    static void setlightDiffuse(float lightDiffuseRIn,float lightDiffuseGIn,float lightDiffuseBIn,float lightDiffuseAIn);
    static void setlightSpecular(float lightSpecularRIn,float lightSpecularGIn,float lightSpecularBIn,float lightSpecularAIn);
    static void getTYJZ();
};
#endif 
