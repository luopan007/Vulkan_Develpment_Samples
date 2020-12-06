#ifndef VULKANEXBASE_TexLightObject_H
#define VULKANEXBASE_TexLightObject_H

#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
#include "TextureManager.h"

class TexLightObject
{
public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;

    int* indexData;
    int indexCount;

    //原始数据相关组-------------------------------
    //顶点数据缓冲
    VkBuffer vertexDatabuf;
    //顶点数据内存
    VkDeviceMemory vertexDataMem;
    //顶点数据Buffer信息
    VkDescriptorBufferInfo vertexDataBufferInfo;

    //计算着色器数据相关组-------------------------------
    //顶点数据缓冲
    VkBuffer vertexDatabufCompute;
    //顶点数据内存
    VkDeviceMemory vertexDataMemCompute;
    //顶点数据Buffer信息
    VkDescriptorBufferInfo vertexDataBufferInfoCompute;

    //顶点索引数据缓冲
    VkBuffer vertexIndexDatabuf;
    //顶点索引数据内存
    VkDeviceMemory vertexIndexDataMem;

    //创建原始数据缓冲相关
    void createVertexDataBuffer(int dataByteCount,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    //创建计算着色器数据缓冲相关
    void createVertexDataBufferCompute(int dataByteCount, VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties);
    //创建索引数据缓冲相关
    void createVertexIndexDataBuffer(int indexDataByteCount,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);

    TexLightObject(float* vdataIn,int vdataByteCount,int vCountIn,int* indexDataIn,int indexDataByteCount,int indexCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~TexLightObject();

    //绘制方法
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
    //基于计算着色器计算波动的方法
    void calSelfBD(VkCommandBuffer& cmd,VkPipelineLayout &pipelineLayout, VkPipeline &pipeline,VkDescriptorSet *desSetPointer);
    //基于计算着色器计算法向量的方法
    void calSelfNormal(VkCommandBuffer& cmd,VkPipelineLayout &pipelineLayout, VkPipeline &pipeline,VkDescriptorSet *desSetPointer);
};


#endif
