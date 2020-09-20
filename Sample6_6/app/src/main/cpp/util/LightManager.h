#ifndef SAMPLE5_1_LIGHTMANAGER_H
#define SAMPLE5_1_LIGHTMANAGER_H
class LightManager
{
public:
    static float lx,ly,lz;
    static float lightAmbientR,lightAmbientG,lightAmbientB,lightAmbientA;
    static float lightDiffuseR,lightDiffuseG,lightDiffuseB,lightDiffuseA;
    static float lightSpecularR,lightSpecularG,lightSpecularB,lightSpecularA;
    static void setLightPosition(float lxIn,float lyIn,float lzIn);
    static void setlightAmbient(float lightAmbientRIn,float lightAmbientGIn,float lightAmbientBIn,float lightAmbientAIn);
    static void setlightDiffuse(float lightDiffuseRIn,float lightDiffuseGIn,float lightDiffuseBIn,float lightDiffuseAIn);
    static void setlightSpecular(float lightSpecularRIn,float lightSpecularGIn,float lightSpecularBIn,float lightSpecularAIn);
    static float lightFWJ;
    static void move(float fwjSpan);
};
#endif 
