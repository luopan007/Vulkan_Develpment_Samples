#ifndef VULKANEXBASE_LIGHTMANAGER_H
#define VULKANEXBASE_LIGHTMANAGER_H
#define LIGHTS_COUNT 4
typedef struct LightDataStruct
{
    float lx = 0, ly = 0, lz = 0,lr = 500;
    float lightAmbientR=0, lightAmbientG=0, lightAmbientB=0, lightAmbientA=0;
    float lightDiffuseR=0, lightDiffuseG=0, lightDiffuseB=0, lightDiffuseA=0;
    float lightSpecularR=0, lightSpecularG=0, lightSpecularB=0, lightSpecularA=0;
} Light;
class LightManager
{
public:
    static Light lightArray[LIGHTS_COUNT];
    static void setLightRange(int id, float lrIn);
    static void setLightPosition(int id,float lxIn,float lyIn,float lzIn);
    static void setlightAmbient(int id,float lightAmbientRIn,float lightAmbientGIn,float lightAmbientBIn,float lightAmbientAIn);
    static void setlightDiffuse(int id,float lightDiffuseRIn,float lightDiffuseGIn,float lightDiffuseBIn,float lightDiffuseAIn);
    static void setlightSpecular(int id,float lightSpecularRIn,float lightSpecularGIn,float lightSpecularBIn,float lightSpecularAIn);
    static float lightFWJ;
    static void move();
};
#endif 
