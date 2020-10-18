#ifndef Square3D_H
#define Square3D_H

#include <android/log.h>
#include <string>
#include "../util/ObjObject.h"
#include "../util/FileUtil.h"
#include "../bndev/MyVulkanManager.h"
using namespace std;//指定使用的命名空间
class Square3D
{
public:
    static ObjObject* create(string objname,string texturename);
};

#endif
