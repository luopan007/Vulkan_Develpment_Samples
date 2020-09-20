#ifndef VULKANEXBASE_OBJOBJECT_H
#define VULKANEXBASE_OBJOBJECT_H
#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
class DrawableObjectCommonLight
{
public:
    float* pushConstantData;							
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    int indirectDrawCount; //间接绘制信息数据组的数量
    int drawCmdbufbytes; //间接绘制信息数据所占总字节数
    VkBuffer drawCmdbuf; //间接绘制信息数据缓冲
    VkDeviceMemory drawCmdMem; //间接绘制信息数据缓冲对应设备内存
    void initDrawCmdbuf(VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties); //用于创建间接绘制信息数据缓冲的方法
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    DrawableObjectCommonLight(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~DrawableObjectCommonLight();
    void drawSelf(VkCommandBuffer& secondary_cmd, VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
};
#endif
