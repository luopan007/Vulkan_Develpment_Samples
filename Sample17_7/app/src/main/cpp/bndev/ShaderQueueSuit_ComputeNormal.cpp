#include "ShaderQueueSuit_ComputeNormal.h"
#include <assert.h>
#include "HelpFunction.h"
#include "mylog.h"
#include "../util/TextureManager.h"
#include "../util/FileUtil.h"
#include "MyVulkanManager.h"
#include "ShaderCompileUtil.h"


void ShaderQueueSuit_ComputeNormal::create_pipeline_layout(VkDevice& device)
{
    
    NUM_DESCRIPTOR_SETS=1;

    
    VkDescriptorSetLayoutBinding layout_bindings[1];
    
    layout_bindings[0].binding = 0;
    layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layout_bindings[0].descriptorCount = 1;
    layout_bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    layout_bindings[0].pImmutableSamplers = NULL;

    
    VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
    descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_layout.pNext = NULL;
    descriptor_layout.bindingCount = 1;
    descriptor_layout.pBindings = layout_bindings;

    
    descLayouts.resize(NUM_DESCRIPTOR_SETS);
    VkResult result = vk::vkCreateDescriptorSetLayout(device, &descriptor_layout, NULL, descLayouts.data());
    assert(result == VK_SUCCESS);

    
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
    pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pPipelineLayoutCreateInfo.pNext = NULL;
    pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pPipelineLayoutCreateInfo.pPushConstantRanges = VK_NULL_HANDLE;
    pPipelineLayoutCreateInfo.setLayoutCount = NUM_DESCRIPTOR_SETS;
    pPipelineLayoutCreateInfo.pSetLayouts = descLayouts.data();
    result = vk::vkCreatePipelineLayout(device, &pPipelineLayoutCreateInfo, NULL, &pipelineLayout);
    assert(result == VK_SUCCESS);
}


void ShaderQueueSuit_ComputeNormal::destroy_pipeline_layout(VkDevice& device)
{
    vk::vkDestroyPipelineLayout(device, pipelineLayout, NULL);
}


void ShaderQueueSuit_ComputeNormal::init_descriptor_set(VkDevice& device)
{
    
    VkDescriptorPoolSize type_count[1];
    type_count[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    type_count[0].descriptorCount = 1;

    
    VkDescriptorPoolCreateInfo descriptor_pool = {};
    descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool.pNext = NULL;
    descriptor_pool.maxSets = 1;
    descriptor_pool.poolSizeCount = 1;
    descriptor_pool.pPoolSizes = type_count;

    
    VkResult result = vk::vkCreateDescriptorPool(device, &descriptor_pool, NULL, &descPool);
    assert(result == VK_SUCCESS);

    
    
    std::vector<VkDescriptorSetLayout> layouts;
    for(int i=0;i<1;i++)
    {
        layouts.push_back(descLayouts[0]);
    }

    
    VkDescriptorSetAllocateInfo alloc_info[1];
    alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info[0].pNext = NULL;
    alloc_info[0].descriptorPool = descPool;
    alloc_info[0].descriptorSetCount = 1;
    alloc_info[0].pSetLayouts = layouts.data();

    
    descSet.resize(1);
    
    result = vk::vkAllocateDescriptorSets(device, alloc_info, descSet.data());
    assert(result == VK_SUCCESS);

    
    writes[0] = {};
    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].pNext = NULL;
    writes[0].descriptorCount = 1;
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writes[0].dstArrayElement = 0;
    writes[0].dstBinding = 0;
}

void ShaderQueueSuit_ComputeNormal::create_shader(VkDevice& device)
{
    std::string compStr= FileUtil::loadAssetStr("shader/computeNormal.comp");

    
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].pNext = NULL;
    shaderStages[0].pSpecializationInfo = NULL;
    shaderStages[0].flags = 0;
    shaderStages[0].stage = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStages[0].pName = "main";

    
    std::vector<unsigned int> vtx_spv;
    bool retVal = GLSLtoSPV(VK_SHADER_STAGE_COMPUTE_BIT, compStr.c_str(), vtx_spv);
    assert(retVal);
    LOGE("计算着色器脚本编译SPV成功！");

    
    VkShaderModuleCreateInfo moduleCreateInfo;
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);
    moduleCreateInfo.pCode = vtx_spv.data();
    
    VkResult result = vk::vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderStages[0].module);
    assert(result == VK_SUCCESS);
}

void ShaderQueueSuit_ComputeNormal::destroy_shader(VkDevice& device)
{
    vk::vkDestroyShaderModule(device, shaderStages[0].module, NULL);
}

void ShaderQueueSuit_ComputeNormal::create_pipe_line(VkDevice& device)
{
    VkComputePipelineCreateInfo pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = NULL;
    pipelineInfo.flags=0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = 0;
    pipelineInfo.layout=pipelineLayout;
    pipelineInfo.stage=shaderStages[0];

    
    VkPipelineCacheCreateInfo pipelineCacheInfo;
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCacheInfo.pNext = NULL;
    pipelineCacheInfo.initialDataSize = 0;
    pipelineCacheInfo.pInitialData = NULL;
    pipelineCacheInfo.flags = 0;
    VkResult result = vk::vkCreatePipelineCache(device, &pipelineCacheInfo, NULL,&pipelineCache);
    assert(result == VK_SUCCESS);

    result = vk::vkCreateComputePipelines(device, pipelineCache, 1, &pipelineInfo, nullptr, &pipeline);
    assert(result == VK_SUCCESS);
}

void ShaderQueueSuit_ComputeNormal::destroy_pipe_line(VkDevice& device)
{
    
    vk::vkDestroyPipeline(device, pipeline, NULL);
    
    vk::vkDestroyPipelineCache(device, pipelineCache, NULL);
}

ShaderQueueSuit_ComputeNormal::ShaderQueueSuit_ComputeNormal(VkDevice* deviceIn,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    this->devicePointer=deviceIn;
    create_shader(*devicePointer);
    create_pipeline_layout(*devicePointer);
    init_descriptor_set(*devicePointer);
    create_pipe_line(*devicePointer);
}

ShaderQueueSuit_ComputeNormal::~ShaderQueueSuit_ComputeNormal()
{
    destroy_pipe_line(*devicePointer);
    destroy_shader(*devicePointer);
    destroy_pipeline_layout(*devicePointer);
}