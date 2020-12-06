#ifndef _CALCULATENORMAL_H
#define _CALCULATENORMAL_H
class CalculateNormal {
public:
    static void calNormal(float* mVectors, //计算顶点法向量数据的方法
                          short* mIndices, int indicesCount, int verNum,float* nXYZ, float* vNormal) ;
    static void getCrossProduct(float x1, //计算向量叉积的方法
                                float y1, float z1, float x2, float y2, float z2, float* result);
    static void vectorNormal(float* vector); //规格化向量的方法
};
#endif
