#ifndef __FileUtil_H__
#define __FileUtil_H__

#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include <string>

using namespace std;//指定使用的命名空间
class FileUtil {
public:
    static AAssetManager *aam;//指向AAssetManager对象的指针
    static void setAAssetManager(AAssetManager *aamIn);//初始化AAssetManager对象
    static string loadAssetStr(string fname);//加载Assets文件夹下的指定文本性质文件内容作为字符串返回
};

#endif