#ifndef VULKANEXBASE_OBJOBJECT_H
#define VULKANEXBASE_OBJOBJECT_H
#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
#include "TextureManager.h"
#include "AABB.h"

class ObjObject
{
public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    AABB* aabb;

    ObjObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~ObjObject();
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
};
#endif
