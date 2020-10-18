#ifndef VULKANEXBASE_OBJOBJECT_H
#define VULKANEXBASE_OBJOBJECT_H

#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
#include "TextureManager.h"
using namespace std;//指定使用的命名空间
class ObjObject
{
public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;
    //坐标
    int x;
    int y;
    int z;
    //旋转
    int Rx;
    int Ry;
    int Rz;
    int Rangle;
    //走向
    int Goorientation;
    //纹理名称
    string texturename;

    //顶点数据缓冲
    VkBuffer vertexDatabuf;
    //顶点数据内存
    VkDeviceMemory vertexDataMem;
    //顶点数据Buffer信息
    VkDescriptorBufferInfo vertexDataBufferInfo;


    ObjObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties,string texturename);
    ~ObjObject();

    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
    void setPosition3D(int x,int y,int z);
};


#endif
