#ifndef MOTIONBLUR_SKYCLOUDDATA_H
#define MOTIONBLUR_SKYCLOUDDATA_H
#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
class SkyCloudData
{
public:
    static float* vdata;
    static int dataByteCount;
    static int vCount;
    static float radius;
    static float* generateTexCoor(int bw,int bh);
    static void initData();
};
#endif 
