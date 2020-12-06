#pragma once
#ifndef FactorManager_H
#define FactorManager_H
class FactorManager {
public:
    static float startX;
    static float endX;
    static float startY;
    static float endY;
    static float startZ;
    static float endZ;
    static float detailDepth;
    static float LFactor;
    static void setFactorValue(float startXIn, float endXIn, float startYIn, float endYIn, float startZIn, float endZIn, float detailDepthIn, float LFactorIn);
    static void setFactorToConstant(float* pushConstantData);
};
#endif 
