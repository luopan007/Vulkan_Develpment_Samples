#ifndef __H__LoadUtil__
#define __H__LoadUtil__//防止重复定义
#include <stdio.h>
#include <string>
#include <vector>
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include "ObjObject.h"
using namespace std;
class LoadUtil {
public:
    static ObjObject *loadFromFile(const string &fname, VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties);//读取obj 文件内容生成绘制用物体对象的方法
};
#endif
