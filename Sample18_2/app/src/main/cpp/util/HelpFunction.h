#ifndef VULKANEXBASE_HELPFUNCTION_H
#define VULKANEXBASE_HELPFUNCTION_H
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
bool memoryTypeFromProperties(VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t typeBits,VkFlags requirements_mask,uint32_t *typeIndex);
void calculateABPosition(
        float x, float y,
        float w, float h,
        float rightIn,
        float topIn,
        float nearIn,
        float farIn,
        float* A,
        float* B);
void calSQ(float x, float y, float* A, float* B);
void changeObj(int index);
#endif
