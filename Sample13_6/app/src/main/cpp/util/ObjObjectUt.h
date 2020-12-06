//
// Created by Administrator on 2018/5/6.
//

#ifndef SAMPLE7_5_OBJOBJECTUT_H
#define SAMPLE7_5_OBJOBJECTUT_H
#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>

class ObjObjectUt {
public:
    float * pushConstantData;
    VkDevice* devicePointer;
    float* vdata;
    int vCount;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;
    ObjObjectUt(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~ObjObjectUt();
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
};


#endif //SAMPLE7_5_OBJOBJECTUT_H
