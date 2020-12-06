#ifndef SAMPLE11_3_TexTrunkDrawableObject_H
#define SAMPLE11_3_TexTrunkDrawableObject_H
#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
#include "TextureManager.h"
class TexTrunkDrawableObject {
public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    float centerx;
    float centery;
    float centerz;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    TexTrunkDrawableObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,
                          VkPhysicalDeviceMemoryProperties& memoryroperties,float tx,float ty,float tz);
    ~TexTrunkDrawableObject();
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
};


#endif //SAMPLE11_3_TexTrunkDrawableObject_H
