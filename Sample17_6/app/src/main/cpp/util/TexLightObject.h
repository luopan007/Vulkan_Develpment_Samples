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
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    VkBuffer vertexDatabufCompute; //计算着色器和绘制共用的顶点数据缓冲
    VkDeviceMemory vertexDataMemCompute; //第4 行顶点数据缓冲对应的设备内存
    VkDescriptorBufferInfo vertexDataBufferInfoCompute; //第4 行顶点数据缓冲描述信息
    VkBuffer vertexIndexDatabuf; //顶点索引数据缓冲
    VkDeviceMemory vertexIndexDataMem; //顶点索引数据缓冲对应的设备内存
    void createVertexDataBuffer(int dataByteCount, //创建存储原始顶点数据数据缓冲的方法
                                VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    void createVertexDataBufferCompute(int dataByteCount, //创建计算着色器及绘制共用数据缓冲的方法
                                       VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties);
    void createVertexIndexDataBuffer(int indexDataByteCount, //创建索引数据缓冲的方法
                                     VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    TexLightObject(float* vdataIn,int vdataByteCount,int vCountIn,int* indexDataIn,int indexDataByteCount,int indexCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~TexLightObject();
    void drawSelf(VkCommandBuffer& cmd, //绘制方法
                  VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
    void calSelfBD(VkCommandBuffer& cmd, //基于计算着色器计算波动的方法
                   VkPipelineLayout &pipelineLayout, VkPipeline &pipeline,VkDescriptorSet *desSetPointer);
    void calSelfNormal(VkCommandBuffer& cmd, //基于计算着色器计算法向量的方法
                       VkPipelineLayout &pipelineLayout, VkPipeline &pipeline,VkDescriptorSet *desSetPointer);

};


#endif
