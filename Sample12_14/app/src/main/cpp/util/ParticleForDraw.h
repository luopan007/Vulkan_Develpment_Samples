#ifndef SAMPLE_SHI_PARTICLEFORDRAW_H
#define SAMPLE_SHI_PARTICLEFORDRAW_H
#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>
#include "TextureManager.h"
class ParticleForDraw {
public:
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    float* pushConstantData;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    ParticleForDraw(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~ParticleForDraw();
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer,int id);
    void updateVertexData(float *vdataIn,int dataByteCount,VkDevice& device);
};
#endif //SAMPLE_SHI_PARTICLEFORDRAW_H
