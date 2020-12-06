#ifndef VULKANEXBASE_HELPFUNCTION_H
#define VULKANEXBASE_HELPFUNCTION_H
#include <vulkan/vulkan.h>
#include <vector>
#include "../vksysutil/vulkan_wrapper.h"
bool memoryTypeFromProperties(VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t typeBits,VkFlags requirements_mask,uint32_t *typeIndex);
float toRadians(float degree);//角度转换成弧度的方法
float toDegrees(float radian);//弧度转换成角度的方法
std::vector<float> generateTexCoor(int bw,int bh);//自动切分纹理产生纹理数组的方法
std::vector<float> generateSkyTexCoor(int bw,int bh);//自动切分天空盒纹理产生纹理数组的方法
float random(float  start, float end);
#endif
