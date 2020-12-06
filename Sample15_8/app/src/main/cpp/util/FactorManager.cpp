#include "FactorManager.h"
float FactorManager::startX = 0.0f;
float FactorManager::endX = 1.0f;
float FactorManager::startY = 0.0f;
float FactorManager::endY = 1.0f;
float FactorManager::startZ = 0.0f;
float FactorManager::endZ = 1.0f;
float FactorManager::detailDepth = 1.0f;
float FactorManager::LFactor = 2.0f;
void FactorManager::setFactorValue(float startXIn, float endXIn, float startYIn, float endYIn, float startZIn, float endZIn, float detailDepthIn, float LFactorIn) {
    startX = startXIn;
    endX = endXIn;
    startY = startYIn;
    endY = endYIn;
    startZ = startZIn;
    endZ = endZIn;
    detailDepth = detailDepthIn;
    LFactor = LFactorIn;
}
void FactorManager::setFactorToConstant(float* pushConstantData) {
    pushConstantData[0] = startX;
    pushConstantData[1] = endX;
    pushConstantData[2] = startY;
    pushConstantData[3] = endY;
    pushConstantData[4] = startZ;
    pushConstantData[5] = endZ;
    pushConstantData[6] = detailDepth;
    pushConstantData[7] = LFactor;
}