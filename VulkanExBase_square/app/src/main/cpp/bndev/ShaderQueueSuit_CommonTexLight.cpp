#include "ShaderQueueSuit_CommonTexLight.h"
#include <assert.h>
#include "HelpFunction.h"
#include "mylog.h"
#include "../util/TextureManager.h"
#include "../util/FileUtil.h"
#include "MyVulkanManager.h"
#include "ShaderCompileUtil.h"

//创建Unifirm Buffer
void ShaderQueueSuit_CommonTexLight::create_uniform_buffer(VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
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
void ShaderQueueSuit_CommonTexLight::destroy_uniform_buffer(VkDevice& device)
{
    vk::vkDestroyBuffer(device, uniformBuf, NULL);
    vk::vkFreeMemory(device, memUniformBuf, NULL);
}

//创建管线layout
void ShaderQueueSuit_CommonTexLight::create_pipeline_layout(VkDevice& device)
{
    //设置DescriptorSet数量
    NUM_DESCRIPTOR_SETS=1;

    //创建VkDescriptorSetLayoutBinding实例数组
    VkDescriptorSetLayoutBinding layout_bindings[2];
    //服务于数据uniform的顶点着色器中的“ binding = 0”
    layout_bindings[0].binding = 0;
    layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layout_bindings[0].descriptorCount = 1;
    layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    layout_bindings[0].pImmutableSamplers = NULL;

    //服务于纹理的 片元着色器中的“binding = 1”
    layout_bindings[1].binding = 1;
    layout_bindings[1].descriptorType =VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layout_bindings[1].descriptorCount = 1;
    layout_bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    layout_bindings[1].pImmutableSamplers = NULL;

    //创建VkDescriptorSetLayoutCreateInfo实例
    VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
    descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_layout.pNext = NULL;
    descriptor_layout.bindingCount = 2;
    descriptor_layout.pBindings = layout_bindings;

    //调整空间
    descLayouts.resize(NUM_DESCRIPTOR_SETS);
    VkResult result = vk::vkCreateDescriptorSetLayout(device, &descriptor_layout, NULL, descLayouts.data());
    assert(result == VK_SUCCESS);

    //推送常量
    const unsigned push_constant_range_count = 1;
    VkPushConstantRange push_constant_ranges[push_constant_range_count] = {};
    push_constant_ranges[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    push_constant_ranges[0].offset = 0;
    push_constant_ranges[0].size = sizeof(float)*16;

    //创建VkPipelineLayoutCreateInfo实例
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
    pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pPipelineLayoutCreateInfo.pNext = NULL;
    pPipelineLayoutCreateInfo.pushConstantRangeCount = push_constant_range_count;
    pPipelineLayoutCreateInfo.pPushConstantRanges = push_constant_ranges;
    pPipelineLayoutCreateInfo.setLayoutCount = NUM_DESCRIPTOR_SETS;
    pPipelineLayoutCreateInfo.pSetLayouts = descLayouts.data();
    result = vk::vkCreatePipelineLayout(device, &pPipelineLayoutCreateInfo, NULL, &pipelineLayout);
    assert(result == VK_SUCCESS);
}

//销毁管线layout
void ShaderQueueSuit_CommonTexLight::destroy_pipeline_layout(VkDevice& device)
{
    for (int i = 0; i < NUM_DESCRIPTOR_SETS; i++)
    {
        vk::vkDestroyDescriptorSetLayout(device, descLayouts[i], NULL);
    }
    vk::vkDestroyPipelineLayout(device, pipelineLayout, NULL);
}

//初始化descriptor set
void ShaderQueueSuit_CommonTexLight::init_descriptor_set(VkDevice& device)
{
    //创建VkDescriptorPoolSize数组
    VkDescriptorPoolSize type_count[2];
    type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    type_count[0].descriptorCount = TextureManager::texNamesForCommonTexLight.size();
    type_count[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    type_count[1].descriptorCount = TextureManager::texNamesForCommonTexLight.size();

    //创建VkDescriptorPoolCreateInfo实例
    VkDescriptorPoolCreateInfo descriptor_pool = {};
    descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool.pNext = NULL;
    descriptor_pool.maxSets = TextureManager::texNamesForCommonTexLight.size();
    descriptor_pool.poolSizeCount = 2;
    descriptor_pool.pPoolSizes = type_count;

    //创建Descriptor池
    VkResult result = vk::vkCreateDescriptorPool(device, &descriptor_pool, NULL, &descPool);
    assert(result == VK_SUCCESS);

    //这里是一个坑，要小心，DescriptorSetLayout只有一个但要用多次
    //每一种着色器的uniform参数结构组合一个DescriptorSetLayout
    std::vector<VkDescriptorSetLayout> layouts;
    for(int i=0;i<TextureManager::texNamesForCommonTexLight.size();i++)
    {
        layouts.push_back(descLayouts[0]);
    }

    //给出描述集的创建信息
    VkDescriptorSetAllocateInfo alloc_info[1];
    alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info[0].pNext = NULL;
    alloc_info[0].descriptorPool = descPool;
    alloc_info[0].descriptorSetCount = TextureManager::texNamesForCommonTexLight.size();
    alloc_info[0].pSetLayouts = layouts.data();

    //调整尺寸
    descSet.resize(TextureManager::texNamesForCommonTexLight.size());
    //分配DescriptorSets
    result = vk::vkAllocateDescriptorSets(device, alloc_info, descSet.data());
    assert(result == VK_SUCCESS);

    //创建VkWriteDescriptorSet数组(顶点与片元着色器中有多少个uniform的块就有多少个writes)
    writes[0] = {};
    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].pNext = NULL;
    writes[0].descriptorCount = 1;
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writes[0].pBufferInfo = &uniformBufferInfo;
    writes[0].dstArrayElement = 0;
    writes[0].dstBinding = 0;

    writes[1] = {};
    writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[1].dstBinding = 1;
    writes[1].descriptorCount = 1;
    writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writes[1].dstArrayElement = 0;
}

void ShaderQueueSuit_CommonTexLight::create_shader(VkDevice& device)
{
    std::string vertStr= FileUtil::loadAssetStr("shader/commonTexLight.vert");
    std::string fragStr= FileUtil::loadAssetStr("shader/commonTexLight.frag");

    //准备顶点着色器阶段信息
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].pNext = NULL;
    shaderStages[0].pSpecializationInfo = NULL;
    shaderStages[0].flags = 0;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].pName = "main";

    //将顶点着色器脚本编译为SPV
    std::vector<unsigned int> vtx_spv;
    bool retVal = GLSLtoSPV(VK_SHADER_STAGE_VERTEX_BIT, vertStr.c_str(), vtx_spv);
    assert(retVal);
    LOGE("顶点着色器脚本编译SPV成功！");

    //准备顶点着色器模块创建信息
    VkShaderModuleCreateInfo moduleCreateInfo;
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);
    moduleCreateInfo.pCode = vtx_spv.data();
    //创建顶点着色器模块
    VkResult result = vk::vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderStages[0].module);
    assert(result == VK_SUCCESS);

    //准备片元着色器阶段信息
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].pNext = NULL;
    shaderStages[1].pSpecializationInfo = NULL;
    shaderStages[1].flags = 0;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].pName = "main";

    //将片元着色器脚本编译为SPV
    std::vector<unsigned int> frag_spv;
    retVal = GLSLtoSPV(VK_SHADER_STAGE_FRAGMENT_BIT, fragStr.c_str(), frag_spv);
    assert(retVal);
    LOGE("片元着色器脚本编译SPV成功！");

    //准备片元着色器模块创建信息
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = frag_spv.size() * sizeof(unsigned int);
    moduleCreateInfo.pCode = frag_spv.data();
    //创建片元着色器模块
    result = vk::vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderStages[1].module);
    assert(result == VK_SUCCESS);
}

void ShaderQueueSuit_CommonTexLight::destroy_shader(VkDevice& device)
{
    vk::vkDestroyShaderModule(device, shaderStages[0].module, NULL);
    vk::vkDestroyShaderModule(device, shaderStages[1].module, NULL);
}

void ShaderQueueSuit_CommonTexLight::initVertexAttributeInfo()
{
//管线的顶点数据输入绑定信息
    vertexBinding.binding = 0;
    vertexBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vertexBinding.stride = sizeof(float)*8;
    //管线的顶点坐标属性信息
    vertexAttribs[0].binding = 0;
    vertexAttribs[0].location = 0;
    vertexAttribs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttribs[0].offset = 0;
    //管线的顶点纹理坐标属性信息
    vertexAttribs[1].binding = 0;
    vertexAttribs[1].location = 1;
    vertexAttribs[1].format = VK_FORMAT_R32G32_SFLOAT;
    vertexAttribs[1].offset = 12;
    //管线的顶点法向量属性信息
    vertexAttribs[2].binding = 0;
    vertexAttribs[2].location = 2;
    vertexAttribs[2].format =  VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttribs[2].offset = 20;
}

void ShaderQueueSuit_CommonTexLight::create_pipe_line(VkDevice& device,VkRenderPass& renderPass)
{
    VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
    memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);

    //管线的动态状态信息
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = NULL;
    dynamicState.pDynamicStates = dynamicStateEnables;
    dynamicState.dynamicStateCount = 0;

    //管线的顶点数据输入阶段信息
    VkPipelineVertexInputStateCreateInfo vi;
    vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vi.pNext = NULL;
    vi.flags = 0;
    vi.vertexBindingDescriptionCount = 1;
    vi.pVertexBindingDescriptions = &vertexBinding;
    vi.vertexAttributeDescriptionCount = 3;
    vi.pVertexAttributeDescriptions =vertexAttribs;

    //管线的图元组装阶段信息
    VkPipelineInputAssemblyStateCreateInfo ia;
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.pNext = NULL;
    ia.flags = 0;
    ia.primitiveRestartEnable = VK_FALSE;
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    //管线的光栅化阶段信息
    VkPipelineRasterizationStateCreateInfo rs;
    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.pNext = NULL;
    rs.flags = 0;
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.cullMode = VK_CULL_MODE_BACK_BIT;
    rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;//卷绕方向
    rs.depthClampEnable = VK_TRUE;
    rs.rasterizerDiscardEnable = VK_FALSE;
    rs.depthBiasEnable = VK_FALSE;
    rs.depthBiasConstantFactor = 0;
    rs.depthBiasClamp = 0;
    rs.depthBiasSlopeFactor = 0;
    rs.lineWidth = 1.0f;

    //混合设置
    VkPipelineColorBlendAttachmentState att_state[1];
    att_state[0].colorWriteMask = 0xf;
    att_state[0].blendEnable = VK_FALSE;
    att_state[0].alphaBlendOp = VK_BLEND_OP_ADD;
    att_state[0].colorBlendOp = VK_BLEND_OP_ADD;
    att_state[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    att_state[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    att_state[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    att_state[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

    //管线的颜色混合阶段信息
    VkPipelineColorBlendStateCreateInfo cb;
    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    cb.pNext = NULL;
    cb.flags = 0;
    cb.attachmentCount = 1;
    cb.pAttachments = att_state;
    cb.logicOpEnable = VK_FALSE;
    cb.logicOp = VK_LOGIC_OP_NO_OP;
    cb.blendConstants[0] = 1.0f;
    cb.blendConstants[1] = 1.0f;
    cb.blendConstants[2] = 1.0f;
    cb.blendConstants[3] = 1.0f;

    //视口
    VkViewport viewports;
    viewports.minDepth = 0.0f;
    viewports.maxDepth = 1.0f;
    viewports.x = 0;
    viewports.y = 0;
    viewports.width = MyVulkanManager::screenWidth;
    viewports.height = MyVulkanManager::screenHeight;

    //剪裁窗口
    VkRect2D scissor;
    scissor.extent.width = MyVulkanManager::screenWidth;
    scissor.extent.height = MyVulkanManager::screenHeight;
    scissor.offset.x = 0;
    scissor.offset.y = 0;

    //管线的视口信息
    VkPipelineViewportStateCreateInfo vp = {};
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.pNext = NULL;
    vp.flags = 0;
    vp.viewportCount = 1;
    vp.scissorCount = 1;
    vp.pScissors = &scissor;
    vp.pViewports = &viewports;

    //管线深度检测模板测试阶段信息
    VkPipelineDepthStencilStateCreateInfo ds;
    ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    ds.pNext = NULL;
    ds.flags = 0;
    ds.depthTestEnable = VK_TRUE;
    ds.depthWriteEnable = VK_TRUE;
    ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    ds.depthBoundsTestEnable = VK_FALSE;
    ds.minDepthBounds = 0;
    ds.maxDepthBounds = 0;
    ds.stencilTestEnable = VK_FALSE;
    ds.back.failOp = VK_STENCIL_OP_KEEP;
    ds.back.passOp = VK_STENCIL_OP_KEEP;
    ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
    ds.back.compareMask = 0;
    ds.back.reference = 0;
    ds.back.depthFailOp = VK_STENCIL_OP_KEEP;
    ds.back.writeMask = 0;
    ds.front = ds.back;

    //管线多重采样信息
    VkPipelineMultisampleStateCreateInfo ms;
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.pNext = NULL;
    ms.flags = 0;
    ms.pSampleMask = NULL;
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    ms.sampleShadingEnable = VK_FALSE;
    ms.alphaToCoverageEnable = VK_FALSE;
    ms.alphaToOneEnable = VK_FALSE;
    ms.minSampleShading = 0.0;

    //管线创建信息
    VkGraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = NULL;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = 0;
    pipelineInfo.flags = 0;
    pipelineInfo.pVertexInputState = &vi;
    pipelineInfo.pInputAssemblyState = &ia;
    pipelineInfo.pRasterizationState = &rs;
    pipelineInfo.pColorBlendState = &cb;
    pipelineInfo.pTessellationState = NULL;
    pipelineInfo.pMultisampleState = &ms;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.pViewportState = &vp;
    pipelineInfo.pDepthStencilState = &ds;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.stageCount = 2;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    //创建管线缓冲
    VkPipelineCacheCreateInfo pipelineCacheInfo;
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCacheInfo.pNext = NULL;
    pipelineCacheInfo.initialDataSize = 0;
    pipelineCacheInfo.pInitialData = NULL;
    pipelineCacheInfo.flags = 0;
    VkResult result = vk::vkCreatePipelineCache(device, &pipelineCacheInfo, NULL,&pipelineCache);
    assert(result == VK_SUCCESS);
    //创建管线
    result = vk::vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineInfo, NULL, &pipeline);
    assert(result == VK_SUCCESS);
}

void ShaderQueueSuit_CommonTexLight::destroy_pipe_line(VkDevice& device)
{
    //销毁管线
    vk::vkDestroyPipeline(device, pipeline, NULL);
    //销毁管线缓冲
    vk::vkDestroyPipelineCache(device, pipelineCache, NULL);
}

ShaderQueueSuit_CommonTexLight::ShaderQueueSuit_CommonTexLight(VkDevice* deviceIn,VkRenderPass& renderPass,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    this->devicePointer=deviceIn;
    create_uniform_buffer(*devicePointer,memoryroperties);
    create_pipeline_layout(*devicePointer);
    init_descriptor_set(*devicePointer);
    create_shader(*devicePointer);
    initVertexAttributeInfo();
    create_pipe_line(*devicePointer,renderPass);
}

ShaderQueueSuit_CommonTexLight::~ShaderQueueSuit_CommonTexLight()
{
    destroy_pipe_line(*devicePointer);
    destroy_shader(*devicePointer);
    destroy_pipeline_layout(*devicePointer);
    destroy_uniform_buffer(*devicePointer);
}