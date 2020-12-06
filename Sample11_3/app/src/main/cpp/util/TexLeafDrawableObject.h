#ifndef SAMPLE11_3_TEXLEAFDRAWABLEOBJECT_H
#define SAMPLE11_3_TEXLEAFDRAWABLEOBJECT_H
#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
#include "TextureManager.h"
class TexLeafDrawableObject {
public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    float postionX;//树的中心X坐标
    float postionY;//树的中心Y坐标
    float postionZ;//树的中心Z坐标
    float centerx;//树叶的中心X坐标
    float centery;//树叶的中心Y坐标
    float centerz;//树叶的中心Z坐标
    float centerAngle;//树叶的旋转角度
    float id;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    TexLeafDrawableObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,
                          VkPhysicalDeviceMemoryProperties& memoryroperties,float px,float py,float pz,float tangle,float idIn);
    ~TexLeafDrawableObject();
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
};


#endif //SAMPLE11_3_TEXLEAFDRAWABLEOBJECT_H
