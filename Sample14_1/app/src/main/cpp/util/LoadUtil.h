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
    static AAssetManager* aam;//指向AAssetManager对象的指针

    static void setAAssetManager(AAssetManager* aamIn);//初始化AAssetManager对象
    static ObjObject* loadFromFile(const string& fname,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);

    static size_t  splitString(const string& strSrc, const string& strDelims, vector<string>& strDest);
    static float parseFloat(const char *token);
    static bool tryParseDouble(const char *s, const char *s_end, double *result);
    static int parseInt(const char *token);
};

#endif 
