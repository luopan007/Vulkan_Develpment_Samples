#ifndef VULKANEXBASE_OBJOBJECT_H
#define VULKANEXBASE_OBJOBJECT_H

#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>

class DrawableObjectCommonLight {
public:
    VkDevice *devicePointer;
    float *vdata;
    int vCount;
    VkBuffer vertexDatabuf;
    VkDeviceMemory vertexDataMem;
    VkDescriptorBufferInfo vertexDataBufferInfo;

    DrawableObjectCommonLight(float *vdataIn, int dataByteCount, int vCountIn, VkDevice &device,
                              VkPhysicalDeviceMemoryProperties &memoryroperties);

    ~DrawableObjectCommonLight();

    void
    drawSelf(VkCommandBuffer &secondary_cmd, VkPipelineLayout &pipelineLayout, VkPipeline &pipeline,
             VkDescriptorSet *desSetPointer);
};

#endif
