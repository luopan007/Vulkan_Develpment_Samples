#ifndef VULKANEXBASE_SHADERCOMPILEUTIL_H
#define VULKANEXBASE_SHADERCOMPILEUTIL_H

#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"


bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader, std::vector<unsigned int> &spirv);

#endif
