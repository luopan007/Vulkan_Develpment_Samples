#ifndef VULKANEXBASE_OBJOBJECT_H
#define VULKANEXBASE_OBJOBJECT_H 
#include <vulkan/vulkan.h>
class DrawableObjectCommonLight{
public:
    
    int indirectDrawCount;
    int drawCmdbufbytes;
    VkBuffer drawCmdbuf;
    VkDeviceMemory drawCmdMem;
    float* pushConstantData; 
    VkDevice* devicePointer; 
    float* vdata; 
    int vCount; 
    uint16_t * idata; 
    int iCount; 
    VkBuffer vertexDatabuf; 
    VkDeviceMemory vertexDataMem; 
    VkDescriptorBufferInfo vertexDataBufferInfo; 
    VkBuffer indexDatabuf; 
    VkDeviceMemory indexDataMem; 
    VkDescriptorBufferInfo indexDataBufferInfo; 
    DrawableObjectCommonLight(float* vdataIn,int dataByteCount,int vCountIn, 
                              uint16_t* idataIn,int indexByteCount,int iCountIn,
                              VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~DrawableObjectCommonLight(); 
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer,int cmdDataOffset);
    void initDrawCmdbuf(VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryroperties);
private:
    void createVertexBuffer(int dataByteCount,VkDevice& device, 
                            VkPhysicalDeviceMemoryProperties& memoryroperties);
    void createIndexBuffer(int indexByteCount,VkDevice& device, 
                           VkPhysicalDeviceMemoryProperties& memoryroperties);
};
#endif
