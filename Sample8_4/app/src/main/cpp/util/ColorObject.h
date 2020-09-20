#ifndef SAMPLE5_1_COLOROBJECT_H
#define SAMPLE5_1_COLOROBJECT_H
#include <vulkan/vulkan.h>
#include <string>
class ColorObject
{
public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    ColorObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~ColorObject();
    void drawSelf(VkCommandBuffer& secondary_cmd, VkPipelineLayout& pipelineLayout,VkPipeline& pipeline);
};
#endif 
