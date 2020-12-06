#ifndef __CalNormal_H__
#define __CalNormal_H__
#include <map>
#include <set>
using namespace std;
class CalNormal{
public :
    static void calNormal(float* vdata, int vCount, int* idata, int iCount, float* vNormals);
    static void getCrossProduct(float x1, float y1, float z1, float x2, float y2, float z2, float* result);
    static void vectorNormal(float* vector); 
    static float* oneNormal;
    static int* index;
};
#endif