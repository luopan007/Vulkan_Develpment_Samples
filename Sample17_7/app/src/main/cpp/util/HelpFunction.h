#ifndef VULKANEXBASE_HELPFUNCTION_H
#define VULKANEXBASE_HELPFUNCTION_H

#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
#include <vector>
#include <string>
bool memoryTypeFromProperties(VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t typeBits,VkFlags requirements_mask,uint32_t *typeIndex);
size_t  splitString(const std::string& strSrc, const std::string& strDelims, std::vector<std::string>& strDest);
#endif
