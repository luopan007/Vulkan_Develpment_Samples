#ifndef VULKANEXBASE_SHADERQUEUESUIT_CommonTexLight_H
#define VULKANEXBASE_SHADERQUEUESUIT_CommonTexLight_H
#include <vector>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
class ShaderQueueSuit_CommonTexLight
{
private:
    VkBuffer uniformBufVert;
    VkDescriptorBufferInfo uniformBufferInfoVert;
    VkBuffer uniformBufFrag;
    VkDescriptorBufferInfo uniformBufferInfoFrag;
    int NUM_DESCRIPTOR_SETS;
    std::vector<VkDescriptorSetLayout> descLayouts;
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkVertexInputBindingDescription vertexBinding;
    VkVertexInputAttributeDescription vertexAttribs[3];
    VkPipelineCache pipelineCache;
    VkDevice* devicePointer;
    VkDescriptorPool descPool;
    void create_uniform_buffer_vert(VkDevice &device,
                                    VkPhysicalDeviceMemoryProperties &memoryroperties);
    void destroy_uniform_buffer_vert(VkDevice &device);
    void create_uniform_buffer_frag(VkDevice &device,
                                    VkPhysicalDeviceMemoryProperties &memoryroperties);
    void destroy_uniform_buffer_frag(VkDevice &device);
    void create_pipeline_layout(VkDevice& device);
    void destroy_pipeline_layout(VkDevice& device);
    void init_descriptor_set(VkDevice& device);
    void create_shader(VkDevice& device);
    void destroy_shader(VkDevice& device);
    void initVertexAttributeInfo();
    void create_pipe_line(VkDevice& device,VkRenderPass& renderPass);
    void destroy_pipe_line(VkDevice& device);
  public:
    VkDeviceMemory memUniformBufVert;
    int bufferByteCountVert;
    VkDeviceMemory memUniformBufFrag;
    int bufferByteCountFrag;
    VkWriteDescriptorSet writes[4];
    std::vector<VkDescriptorSet> descSet;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    ShaderQueueSuit_CommonTexLight(VkDevice* deviceIn,VkRenderPass& renderPass,VkPhysicalDeviceMemoryProperties& memoryroperties);
    ~ShaderQueueSuit_CommonTexLight();
};
#endif
