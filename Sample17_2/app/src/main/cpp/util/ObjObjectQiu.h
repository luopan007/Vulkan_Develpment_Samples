#ifndef VULKANEXBASE_ObjObjectQiu_H
#define VULKANEXBASE_ObjObjectQiu_H
#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
class ObjObjectQiu
{
public:
    float * pushConstantData;
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    ObjObjectQiu(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~ObjObjectQiu();
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
};
#endif
