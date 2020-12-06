#ifndef VULKANEXBASE_TEXTURERECT_H
#define VULKANEXBASE_TEXTURERECT_H
#include <vulkan/vulkan.h>
#include <string>
#include "TextureManager.h"
class TextureRect
{
public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    TextureRect(float* vdataIn, int dataByteCount, int vCountIn, VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~TextureRect();
    void drawSelf(VkCommandBuffer& cmd, VkPipelineLayout& pipelineLayout, VkPipeline& pipeline, VkDescriptorSet* desSetPointer);
};
#endif
