#include <mylog.h>
#include "Square3D.h"
#include "../util/LoadUtil.h"
#include "../bndev/MyVulkanManager.h"

//加载非压缩RGBA四通道普通纹理数据
ObjObject* Square3D::create(string objname, string texturename) {
    ObjObject* tempOO=LoadUtil::loadFromFile(objname,MyVulkanManager::device, MyVulkanManager::memoryroperties);
    tempOO->texturename=texturename;
    return tempOO;
}
