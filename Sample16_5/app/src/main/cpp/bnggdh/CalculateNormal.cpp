#include <math.h>
#include "CalculateNormal.h"
#include "string.h"
void CalculateNormal::calNormal(float* mVectors, //用于计算法向量的方法
                                short* mIndices, int indicesCount, int verNum,float* nXYZ, float* vNormal) {
    memset(nXYZ, 0, verNum * 3 * 4); //将每个顶点的法向量数据清零
    for(int i = 0; i < indicesCount / 3 ; ++i) { //对模型中的每个面进行遍历
        short index_mIn_0 = mIndices[i * 3]; //该面的第一个顶点索引
        short index_mIn_1 = mIndices[i * 3 + 1]; //该面的第二个顶点索引
        short index_mIn_2 = mIndices[i * 3 + 2]; //该面的第三个顶点索引
        float x0 = mVectors[index_mIn_0 * 3]; //第一个顶点的x 坐标
        float y0 = mVectors[index_mIn_0 * 3 + 1]; //第一个顶点的y 坐标
        float z0 = mVectors[index_mIn_0 * 3 + 2]; //第一个顶点的z 坐标
        float x1 = mVectors[index_mIn_1 * 3]; //第二个顶点的x 坐标
        float y1 = mVectors[index_mIn_1 * 3 + 1]; //第二个顶点的y 坐标
        float z1 = mVectors[index_mIn_1 * 3 + 2]; //第二个顶点的z 坐标
        float x2 = mVectors[index_mIn_2 * 3]; //第三个顶点的x 坐标
        float y2 = mVectors[index_mIn_2 * 3 + 1]; //第三个顶点的y 坐标
        float z2 = mVectors[index_mIn_2 * 3 + 2]; //第三个顶点的z 坐标
        float vxa = x1 - x0;float vya = y1 - y0;float vza = z1 - z0; //第二个点到第一个点的向量
        float vxb = x2 - x0;float vyb = y2 - y0;float vzb = z2 - z0; //第三个点到第一个点的向量
        getCrossProduct(vxa, vya, vza, vxb, vyb, vzb, vNormal); //求两个向量的叉积
        vectorNormal(vNormal); //规格化向量
        nXYZ[index_mIn_0 * 3 + 0] += vNormal[0]; //叠加第一个顶点法向量的X 分量
        nXYZ[index_mIn_0 * 3 + 1] += vNormal[1]; //叠加第一个顶点法向量的Y 分量
        nXYZ[index_mIn_0 * 3 + 2] += vNormal[2]; //叠加第一个顶点法向量的Z 分量
        nXYZ[index_mIn_1 * 3 + 0] += vNormal[0]; //叠加第二个顶点法向量的X 分量
        nXYZ[index_mIn_1 * 3 + 1] += vNormal[1]; //叠加第二个顶点法向量的Y 分量
        nXYZ[index_mIn_1 * 3 + 2] += vNormal[2]; //叠加第二个顶点法向量的Z 分量
        nXYZ[index_mIn_2 * 3 + 0] += vNormal[0]; //叠加第三个顶点法向量的X 分量
        nXYZ[index_mIn_2 * 3 + 1] += vNormal[1]; //叠加第三个顶点法向量的Y 分量
        nXYZ[index_mIn_2 * 3 + 2] += vNormal[2]; //叠加第三个顶点法向量的Z 分量
    }}
void CalculateNormal::getCrossProduct(float x1, float y1, float z1, float x2, float y2, float z2, float* result) {
    float A = y1 * z2 - y2 * z1;
    float B = z1 * x2 - z2 * x1;
    float C = x1 * y2 - x2 * y1;
    result[0] = A;
    result[1] = B;
    result[2] = C;
}
void CalculateNormal::vectorNormal(float* vector) {
    float module = (float)sqrt((double)(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]));
    vector[0] /= module;
    vector[1] /= module;
    vector[2] /= module;
}
