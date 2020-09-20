
#ifndef SAMPLE5_1_TEXLIGHTOBJECT_H
#define SAMPLE5_1_TEXLIGHTOBJECT_H
#include <vulkan/vulkan.h>
#include <string>
#include "TextureManager.h"
class TexLightObject
{
public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    TexLightObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~TexLightObject();
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
};
#endif 
