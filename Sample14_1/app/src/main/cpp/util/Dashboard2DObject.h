#ifndef VULKANEXBASE_DASHBOARD2DOBJECT_H
#define VULKANEXBASE_DASHBOARD2DOBJECT_H

#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
#include "TextureManager.h"

class Dashboard2DObject
{
  public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;

    //顶点数据缓冲
    VkBuffer vertexDatabuf;
    //顶点数据内存
    VkDeviceMemory vertexDataMem;
    //顶点数据Buffer信息
    VkDescriptorBufferInfo vertexDataBufferInfo;


    Dashboard2DObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~Dashboard2DObject();

    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);

};


#endif
