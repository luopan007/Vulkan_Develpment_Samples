#ifndef __H__LoadUtil__
#define __H__LoadUtil__
#include <stdio.h>
#include <string>
#include <vector>
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include "ObjObject.h"
using namespace std;
class LoadUtil
{
public:
    static AAssetManager* aam;
    static void setAAssetManager(AAssetManager* aamIn);
    static ObjObject* loadFromFile(const string& fname,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    static float* getCrossProduct(float x1,float y1,float z1,float x2,float y2,float z2);
    static float* vectorNormal(float vector[]);
    static size_t  splitString(const string& strSrc, const string& strDelims, vector<string>& strDest);
    static float parseFloat(const char *token);
    static bool tryParseDouble(const char *s, const char *s_end, double *result);
    static int parseInt(const char *token);
};
#endif 
