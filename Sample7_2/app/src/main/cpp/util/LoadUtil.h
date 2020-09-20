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
    static ObjObject* loadFromFile(const string& fname,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);

};
#endif 
