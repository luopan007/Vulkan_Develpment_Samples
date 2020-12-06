#ifndef SAMPLE17_3_SKYCLOUD_H
#define SAMPLE17_3_SKYCLOUD_H
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
