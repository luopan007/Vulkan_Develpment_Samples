#ifndef VULKANEXBASE_SHADERQUEUESUIT_Dashboard2D_H
#define VULKANEXBASE_SHADERQUEUESUIT_Dashboard2D_H

#include <vector>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"

class ShaderQueueSuit_Dashboard2D
{
private:
    VkBuffer uniformBuf;
    VkDescriptorBufferInfo uniformBufferInfo;
    //DescriptorSet数量
    int NUM_DESCRIPTOR_SETS;
    //DescriptorSetLayout列表
    std::vector<VkDescriptorSetLayout> descLayouts;
    //着色器阶段数组 长度为2 表示仅有 顶点着色器与片元着色器
    VkPipelineShaderStageCreateInfo shaderStages[2];
    //管线的顶点数据输入绑定描述--挂接着色器
    VkVertexInputBindingDescription vertexBinding;
    //管线的顶点数据输入属性描述--挂接着色器
    VkVertexInputAttributeDescription vertexAttribs[2];
    //管线缓冲
    VkPipelineCache pipelineCache;
    //设备
    VkDevice* devicePointer;
    //描述池
    VkDescriptorPool descPool;

    void create_uniform_buffer(VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    void destroy_uniform_buffer(VkDevice& device);
    void create_pipeline_layout(VkDevice& device);
    void destroy_pipeline_layout(VkDevice& device);
    void init_descriptor_set(VkDevice& device);
    void create_shader(VkDevice& device);
    void destroy_shader(VkDevice& device);
    //初始化顶点着色器输入变量信息
    void initVertexAttributeInfo();
    void create_pipe_line(VkDevice& device,VkRenderPass& renderPass);
    void destroy_pipe_line(VkDevice& device);

  public:
    VkDeviceMemory memUniformBuf;
    int bufferByteCount;
    //Uniform写入用描述集
    VkWriteDescriptorSet writes[2];
    //描述集合列表
    std::vector<VkDescriptorSet> descSet;
    //管线布局
    VkPipelineLayout pipelineLayout;
    //管线
    VkPipeline pipeline;

    ShaderQueueSuit_Dashboard2D(VkDevice* deviceIn,VkRenderPass& renderPass,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~ShaderQueueSuit_Dashboard2D();
};

#endif
