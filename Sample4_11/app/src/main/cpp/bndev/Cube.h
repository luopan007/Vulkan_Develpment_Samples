//
// Created by Administrator on 2017/10/30.
//

#ifndef VULKANEXBASE_CUBE_H
#define VULKANEXBASE_CUBE_H

#include "ColorRect.h"
#include "DrawableObjectCommon.h"

class Cube{
public:
    void drawSelf(VkCommandBuffer cmd, //绘制方法
                  VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
    Cube(VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties); //构造函数
    ~Cube(); //析构函数
};
#endif

