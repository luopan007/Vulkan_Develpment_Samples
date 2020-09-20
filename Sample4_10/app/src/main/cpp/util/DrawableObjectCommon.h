#ifndef VULKANEXBASE_OBJOBJECT_H
#define VULKANEXBASE_OBJOBJECT_H //防止重复定义

#include <vulkan/vulkan.h>

class DrawableObjectCommonLight{
public:
    float* pushConstantData; //推送常量数据指针

    VkDevice* devicePointer; //指向逻辑设备的指针
    float* vdata; //顶点数据数组首地址指针
    int vCount; //顶点数量
    uint16_t * idata; //索引数据数组首地址指针
    int iCount; //索引数量
    VkBuffer vertexDatabuf; //顶点数据缓冲
    VkDeviceMemory vertexDataMem; //顶点数据所需设备内存
    VkDescriptorBufferInfo vertexDataBufferInfo; //顶点数据缓冲描述信息
    VkBuffer indexDatabuf; //索引数据缓冲
    VkDeviceMemory indexDataMem; //索引数据所需设备内存
    VkDescriptorBufferInfo indexDataBufferInfo; //索引数据缓冲描述信息
    DrawableObjectCommonLight(float* vdataIn,int dataByteCount,int vCountIn, //构造函数
                              uint16_t* idataIn,int indexByteCount,int iCountIn,
                              VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~DrawableObjectCommonLight(); //析构函数
    void drawSelf(VkCommandBuffer& secondary_cmd,VkPipelineLayout& pipelineLayout,//绘制方法
                  VkPipeline& pipeline,VkDescriptorSet* desSetPointer,uint32_t sIndex,uint32_t eIndex);
private:
    void createVertexBuffer(int dataByteCount,VkDevice& device, //创建顶点数据缓冲的方法
                            VkPhysicalDeviceMemoryProperties& memoryroperties);
    void createIndexBuffer(int indexByteCount,VkDevice& device, //创建索引数据缓冲的方法
                           VkPhysicalDeviceMemoryProperties& memoryroperties);
};
#endif
