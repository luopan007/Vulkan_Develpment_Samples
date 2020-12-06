#include <string.h>
#include <math.h>
#include "CalNormal.h"
float* CalNormal::oneNormal = new float[3]();
int* CalNormal::index = new int[3]();
void CalNormal::calNormal(float* vdata, int vCount, int* idata, int iCount, float* vNormals)
{
    memset(vNormals, 0, vCount * 3 * sizeof(float));
    for (int i = 0; i < iCount / 3; i++)
    {
        index[0] = idata[i * 3 + 0];
        index[1] = idata[i * 3 + 1];
        index[2] = idata[i * 3 + 2];
        float x0 = vdata[index[0] * 8 + 0];
        float y0 = vdata[index[0] * 8 + 1];
        float z0 = vdata[index[0] * 8 + 2];
        float x1 = vdata[index[1] * 8 + 0];
        float y1 = vdata[index[1] * 8 + 1];
        float z1 = vdata[index[1] * 8 + 2];
        float x2 = vdata[index[2] * 8 + 0];
        float y2 = vdata[index[2] * 8 + 1];
        float z2 = vdata[index[2] * 8 + 2];
        float vxa = x1 - x0;
        float vya = y1 - y0;
        float vza = z1 - z0;
        float vxb = x2 - x0;
        float vyb = y2 - y0;
        float vzb = z2 - z0;
        getCrossProduct(vxa, vya, vza, vxb, vyb, vzb, oneNormal);
        vectorNormal(oneNormal);
        vNormals[index[0] * 3 + 0] += oneNormal[0];
        vNormals[index[0] * 3 + 1] += oneNormal[1];
        vNormals[index[0] * 3 + 2] += oneNormal[2];
        vNormals[index[1] * 3 + 0] += oneNormal[0];
        vNormals[index[1] * 3 + 1] += oneNormal[1];
        vNormals[index[1] * 3 + 2] += oneNormal[2];
        vNormals[index[2] * 3 + 0] += oneNormal[0];
        vNormals[index[2] * 3 + 1] += oneNormal[1];
        vNormals[index[2] * 3 + 2] += oneNormal[2];
    }
}
void CalNormal::getCrossProduct(float x1, float y1, float z1, float x2, float y2, float z2, float* result) {
    float A = y1 * z2 - y2 * z1;
    float B = z1 * x2 - z2 * x1;
    float C = x1 * y2 - x2 * y1;
    result[0] = A;
    result[1] = B;
    result[2] = C;
}
void CalNormal::vectorNormal(float* vector) {
    float module = (float)sqrt((double)(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]));
    vector[0] /= module;
    vector[1] /= module;
    vector[2] /= module;
}