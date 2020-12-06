#ifndef VULKANEXBASE_SHADERQUEUESUIT_Light_H
#define VULKANEXBASE_SHADERQUEUESUIT_Light_H
#include <vector>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
class ShaderQueueSuit_Light
{
private:
    VkBuffer uniformBuf;
    VkDescriptorBufferInfo uniformBufferInfo;
    std::vector<VkDescriptorSetLayout> descLayouts;
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkPipelineCache pipelineCache;
    VkDevice* devicePointer;
    VkDescriptorPool descPool;
	VkDescriptorImageInfo colorImageInfo;
	VkDescriptorImageInfo positionImageInfo;
	VkDescriptorImageInfo normalImageInfo;
    void create_uniform_buffer(VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties);
    void destroy_uniform_buffer(VkDevice& device);
    void create_pipeline_layout(VkDevice& device);
    void destroy_pipeline_layout(VkDevice& device);
    void init_descriptor_set(VkDevice& device);
    void create_shader(VkDevice& device);
    void destroy_shader(VkDevice& device);
    void create_pipe_line(VkDevice& device,VkRenderPass& renderPass);
    void destroy_pipe_line(VkDevice& device);
  public:
    VkDeviceMemory memUniformBuf;
    int bufferByteCount;
    VkWriteDescriptorSet writes[4];
    std::vector<VkDescriptorSet> descSet;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    ShaderQueueSuit_Light(VkDevice* deviceIn,VkRenderPass& renderPass,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~ShaderQueueSuit_Light();
};
#endif
