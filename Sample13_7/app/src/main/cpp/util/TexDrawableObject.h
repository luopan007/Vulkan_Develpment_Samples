#ifndef VULKANEXBASE_TexOnlyObject_H
#define VULKANEXBASE_TexOnlyObject_H

#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
#include "TextureManager.h"
#include <vector>
using namespace std;
class TexDrawableObject
{
  public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;


    VkBuffer vertexDatabuf;

    VkDeviceMemory vertexDataMem;

    VkDescriptorBufferInfo vertexDataBufferInfo;


    TexDrawableObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~TexDrawableObject();

    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer,vector<float>& color);

};


#endif
