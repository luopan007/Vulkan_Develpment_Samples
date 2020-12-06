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
    int vdataByteCount;
    float* pushConstantData;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    VkMemoryRequirements vdataMem_reqs;
    int* indexData;
    int indexCount;
    VkBuffer vertexIndexDatabuf;
    VkDeviceMemory vertexIndexDataMem;
    VkDescriptorBufferInfo vertexIndexDataBufferInfo;
    float* vdataForCal; //指向用于计算的顶点位置坐标数据数组首地址的指针
    float* ndataForCal; //指向用于计算的顶点法向量数据数组首地址的指针
    float* vdataForTrans; //指向中间传输区顶点数据数组首地址的指针
    float* zero1; //指向1 号波振源位置坐标数据数组首地址的指针
    float* zero2; //指向2 号波振源位置坐标数据数组首地址的指针
    float* zero3; //指向3 号波振源位置坐标数据数组首地址的指针
    float mytime = 0; //计时器
    TextureRect(float* vdataIn,int dataByteCount,int vCountIn,int* indexDataIn,int indexDataByteCount,
                int indexCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~TextureRect(); //析构函数
    void createVertexDataBuffer(int dataByteCount, //创建顶点数据缓冲的方法
                                VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryroperties);
    void createVertexIndexDataBuffer(int indexDataByteCount, //创建索引数据缓冲的方法
                                     VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryroperties);
    void drawSelf(VkCommandBuffer& cmd, VkPipelineLayout& //绘制方法
    pipelineLayout,VkPipeline& pipeline, VkDescriptorSet* desSetPointer);
    void calVerticesNormalAndTangent(); //计算顶点位置坐标和法向量数据的方法
    float findHeight(float x, float z); //计算3 个波影响后的顶点高度值（y 坐标值）的方法
    void copyData(); //将顶点数据复制进中间传输区的方法
    void referVertexBuffer(); //将顶点数据拷贝到绘制用内存的方法
};
#endif
