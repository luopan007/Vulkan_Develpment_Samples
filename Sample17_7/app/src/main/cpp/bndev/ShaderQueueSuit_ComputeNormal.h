#ifndef VULKANEXBASE_ShaderQueueSuit_ComputeB_H
#define VULKANEXBASE_ShaderQueueSuit_ComputeB_H

#include <vector>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"

//计算波动后法向量的管线相关类
class ShaderQueueSuit_ComputeNormal
{
private:
    //DescriptorSet数量
    int NUM_DESCRIPTOR_SETS;
    //DescriptorSetLayout列表
    std::vector<VkDescriptorSetLayout> descLayouts;
    //着色器阶段数组 长度为1 表示仅有 计算着色器
    VkPipelineShaderStageCreateInfo shaderStages[1];
    //管线缓冲
    VkPipelineCache pipelineCache;
    //设备
    VkDevice* devicePointer;
    //描述池
    VkDescriptorPool descPool;

    void create_pipeline_layout(VkDevice& device);
    void destroy_pipeline_layout(VkDevice& device);
    void init_descriptor_set(VkDevice& device);
    void create_shader(VkDevice& device);
    void destroy_shader(VkDevice& device);
    void create_pipe_line(VkDevice& device);
    void destroy_pipe_line(VkDevice& device);

  public:
    //Uniform写入用描述集
    VkWriteDescriptorSet writes[1];
    //描述集合列表
    std::vector<VkDescriptorSet> descSet;
    //管线布局
    VkPipelineLayout pipelineLayout;
    //管线
    VkPipeline pipeline;

    ShaderQueueSuit_ComputeNormal(VkDevice* deviceIn,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~ShaderQueueSuit_ComputeNormal();
};

#endif
