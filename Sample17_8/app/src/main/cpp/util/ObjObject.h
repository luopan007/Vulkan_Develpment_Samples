#ifndef VULKANEXBASE_OBJOBJECT_H
#define VULKANEXBASE_OBJOBJECT_H
#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
class ObjObject
{
public:
    float * pushConstantData;
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    ObjObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~ObjObject();
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer,
                  float* mvp, float* currMatrix);
};
#endif
