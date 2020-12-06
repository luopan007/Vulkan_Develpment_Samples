#include "ShaderQueueSuit_ComputeBD.h"
#include <assert.h>
#include "HelpFunction.h"
#include "mylog.h"
#include "../util/TextureManager.h"
#include "../util/FileUtil.h"
#include "MyVulkanManager.h"
#include "ShaderCompileUtil.h"

//创建Unifirm Buffer
void ShaderQueueSuit_ComputeBD::create_uniform_buffer(VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    //uniformBuffer的字节数,与着色器中的uniform块里面的字节总数一致
    bufferByteCount=sizeof(float)*16;

    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    buf_info.size = bufferByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(device, &buf_info, NULL, &uniformBuf);
    assert(result == VK_SUCCESS);
    //获取此缓冲的内存需求
    VkMemoryRequirements mem_reqs;
    vk::vkGetBufferMemoryRequirements(device, uniformBuf, &mem_reqs);

    //创建VkMemoryAllocateInfo实例
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = mem_reqs.size;
    //需要的内存类型掩码，0表示没有特殊需要
    VkFlags requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    //获取所需内存类型索引
    bool flag=memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits,requirements_mask, &alloc_info.memoryTypeIndex);
    if(flag){LOGE("确定内存类型成功 类型索引为%d",alloc_info.memoryTypeIndex);}else{LOGE("确定内存类型失败!");}

    //分配内存
    result = vk::vkAllocateMemory(device, &alloc_info, NULL, &memUniformBuf);
    assert(result == VK_SUCCESS);

    //将内存和对应缓冲绑定
    result = vk::vkBindBufferMemory(device, uniformBuf, memUniformBuf, 0);
    assert(result == VK_SUCCESS);
    //记录Buffer Info
    uniformBufferInfo.buffer = uniformBuf;
    uniformBufferInfo.offset = 0;
    uniformBufferInfo.range = bufferByteCount;
}

//销毁Unifirm Buffer相关
void ShaderQueueSuit_ComputeBD::destroy_uniform_buffer(VkDevice& device)
{
    vk::vkDestroyBuffer(device, uniformBuf, NULL);
    vk::vkFreeMemory(device, memUniformBuf, NULL);
}

//创建管线layout
void ShaderQueueSuit_ComputeBD::create_pipeline_layout(VkDevice& device)
{
    //设置DescriptorSet数量
    NUM_DESCRIPTOR_SETS=1;

    //创建VkDescriptorSetLayoutBinding实例数组
    VkDescriptorSetLayoutBinding layout_bindings[3];
    //服务于数据uniform的顶点着色器中的“ binding = 0”
    layout_bindings[0].binding = 0;
    layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layout_bindings[0].descriptorCount = 1;
    layout_bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    layout_bindings[0].pImmutableSamplers = NULL;

    //服务于纹理的 片元着色器中的“binding = 1”
    layout_bindings[1].binding = 1;
    layout_bindings[1].descriptorType =VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layout_bindings[1].descriptorCount = 1;
    layout_bindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    layout_bindings[1].pImmutableSamplers = NULL;

    layout_bindings[2].binding = 2;
    layout_bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layout_bindings[2].descriptorCount = 1;
    layout_bindings[2].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    layout_bindings[2].pImmutableSamplers = NULL;

    //创建VkDescriptorSetLayoutCreateInfo实例
    VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
    descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_layout.pNext = NULL;
    descriptor_layout.bindingCount = 3;
    descriptor_layout.pBindings = layout_bindings;

    //调整空间
    descLayouts.resize(NUM_DESCRIPTOR_SETS);
    VkResult result = vk::vkCreateDescriptorSetLayout(device, &descriptor_layout, NULL, descLayouts.data());
    assert(result == VK_SUCCESS);

    //创建VkPipelineLayoutCreateInfo实例
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

//销毁管线layout
void ShaderQueueSuit_ComputeBD::destroy_pipeline_layout(VkDevice& device)
{
    vk::vkDestroyPipelineLayout(device, pipelineLayout, NULL);
}

//初始化descriptor set
void ShaderQueueSuit_ComputeBD::init_descriptor_set(VkDevice& device)
{
    //创建VkDescriptorPoolSize数组
    VkDescriptorPoolSize type_count[3];
    type_count[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    type_count[0].descriptorCount = 1;
    type_count[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    type_count[1].descriptorCount = 1;
    type_count[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    type_count[2].descriptorCount = 1;

    //创建VkDescriptorPoolCreateInfo实例
    VkDescriptorPoolCreateInfo descriptor_pool = {};
    descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool.pNext = NULL;
    descriptor_pool.maxSets = 1;
    descriptor_pool.poolSizeCount = 3;
    descriptor_pool.pPoolSizes = type_count;

    //创建Descriptor池
    VkResult result = vk::vkCreateDescriptorPool(device, &descriptor_pool, NULL, &descPool);
    assert(result == VK_SUCCESS);

    //这里是一个坑，要小心，DescriptorSetLayout只有一个但要用多次
    //每一种着色器的uniform参数结构组合一个DescriptorSetLayout
    std::vector<VkDescriptorSetLayout> layouts;
    for(int i=0;i<1;i++)
    {
        layouts.push_back(descLayouts[0]);
    }

    //给出描述集的创建信息
    VkDescriptorSetAllocateInfo alloc_info[1];
    alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info[0].pNext = NULL;
    alloc_info[0].descriptorPool = descPool;
    alloc_info[0].descriptorSetCount = 1;
    alloc_info[0].pSetLayouts = layouts.data();

    //调整尺寸
    descSet.resize(1);
    //分配DescriptorSets
    result = vk::vkAllocateDescriptorSets(device, alloc_info, descSet.data());
    assert(result == VK_SUCCESS);

    //创建VkWriteDescriptorSet数组(顶点与片元着色器中有多少个uniform的块就有多少个writes)
    writes[0] = {};
    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].pNext = NULL;
    writes[0].descriptorCount = 1;
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writes[0].dstArrayElement = 0;
    writes[0].dstBinding = 0;

    writes[1] = {};
    writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[1].pNext = NULL;
    writes[1].descriptorCount = 1;
    writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writes[1].dstArrayElement = 0;
    writes[1].dstBinding = 1;

    writes[2] = {};
    writes[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[2].pNext = NULL;
    writes[2].descriptorCount = 1;
    writes[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writes[2].pBufferInfo = &uniformBufferInfo;
    writes[2].dstArrayElement = 0;
    writes[2].dstBinding = 2;
}

void ShaderQueueSuit_ComputeBD::create_shader(VkDevice& device)
{
    std::string compStr= FileUtil::loadAssetStr("shader/computeBD.comp");

    //准备计算着色器阶段信息
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].pNext = NULL;
    shaderStages[0].pSpecializationInfo = NULL;
    shaderStages[0].flags = 0;
    shaderStages[0].stage = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStages[0].pName = "main";

    //将计算着色器脚本编译为SPV
    std::vector<unsigned int> vtx_spv;
    bool retVal = GLSLtoSPV(VK_SHADER_STAGE_COMPUTE_BIT, compStr.c_str(), vtx_spv);
    assert(retVal);
    LOGE("计算着色器脚本编译SPV成功！");

    //准备计算着色器模块创建信息
    VkShaderModuleCreateInfo moduleCreateInfo;
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);
    moduleCreateInfo.pCode = vtx_spv.data();
    //创建计算着色器模块
    VkResult result = vk::vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderStages[0].module);
    assert(result == VK_SUCCESS);
}

void ShaderQueueSuit_ComputeBD::destroy_shader(VkDevice& device)
{
    vk::vkDestroyShaderModule(device, shaderStages[0].module, NULL);
}

void ShaderQueueSuit_ComputeBD::create_pipe_line(VkDevice& device)
{
    VkComputePipelineCreateInfo pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = NULL;
    pipelineInfo.flags=0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = 0;
    pipelineInfo.layout=pipelineLayout;
    pipelineInfo.stage=shaderStages[0];

    //创建管线缓冲
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

void ShaderQueueSuit_ComputeBD::destroy_pipe_line(VkDevice& device)
{
    //销毁管线
    vk::vkDestroyPipeline(device, pipeline, NULL);
    //销毁管线缓冲
    vk::vkDestroyPipelineCache(device, pipelineCache, NULL);
}

ShaderQueueSuit_ComputeBD::ShaderQueueSuit_ComputeBD(VkDevice* deviceIn,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    this->devicePointer=deviceIn;
    create_uniform_buffer(*devicePointer,memoryroperties);
    create_shader(*devicePointer);
    create_pipeline_layout(*devicePointer);
    init_descriptor_set(*devicePointer);
    create_pipe_line(*devicePointer);
}

ShaderQueueSuit_ComputeBD::~ShaderQueueSuit_ComputeBD()
{
    destroy_pipe_line(*devicePointer);
    destroy_shader(*devicePointer);
    destroy_pipeline_layout(*devicePointer);
    destroy_uniform_buffer(*devicePointer);
}