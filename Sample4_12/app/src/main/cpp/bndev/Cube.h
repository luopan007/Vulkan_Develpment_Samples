//
// Created by Administrator on 2017/10/30.
//

#ifndef VULKANEXBASE_CUBE_H
#define VULKANEXBASE_CUBE_H

#include "ColorRect.h"
#include "DrawableObjectCommon.h"

class Cube
{
public:
    DrawableObjectCommon *colorRect;
    float unit_size;

    void drawSelf(VkCommandBuffer cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
    Cube(VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties,float *vdata,float unit_sizeIn);
    ~Cube();
};

#endif //VULKANEXBASE_CUBE_H

