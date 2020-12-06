#ifndef VULKANEXBASE_DASHBOARD2DOBJECT_H
#define VULKANEXBASE_DASHBOARD2DOBJECT_H
#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
#include "TextureManager.h"
class DrawableObject
{
  public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    DrawableObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~DrawableObject();
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
};
#endif
