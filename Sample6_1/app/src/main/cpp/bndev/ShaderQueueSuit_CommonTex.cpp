#include "ShaderQueueSuit_CommonTex.h"
#include <assert.h>
#include "HelpFunction.h"
#include "mylog.h"
#include "../util/TextureManager.h"
#include "../util/FileUtil.h"
#include "MyVulkanManager.h"
#include <shaderc/shaderc.hpp>
#include "ShaderCompileUtil.h"
void ShaderQueueSuit_CommonTex::create_uniform_buffer(VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    bufferByteCount=sizeof(float);
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
    VkMemoryRequirements mem_reqs;
    vk::vkGetBufferMemoryRequirements(device, uniformBuf, &mem_reqs);
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = mem_reqs.size;
    VkFlags requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bool flag=memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits,requirements_mask, &alloc_info.memoryTypeIndex);
    if(flag){LOGE("确定内存类型成功 类型索引为%d",alloc_info.memoryTypeIndex);}else{LOGE("确定内存类型失败!");}
    result = vk::vkAllocateMemory(device, &alloc_info, NULL, &memUniformBuf);
    assert(result == VK_SUCCESS);
    result = vk::vkBindBufferMemory(device, uniformBuf, memUniformBuf, 0);
    assert(result == VK_SUCCESS);
    uniformBufferInfo.buffer = uniformBuf;
    uniformBufferInfo.offset = 0;
    uniformBufferInfo.range = bufferByteCount;
}
void ShaderQueueSuit_CommonTex::destroy_uniform_buffer(VkDevice& device)
{
    vk::vkDestroyBuffer(device, uniformBuf, NULL);
    vk::vkFreeMemory(device, memUniformBuf, NULL);
}
void ShaderQueueSuit_CommonTex::create_pipeline_layout(VkDevice& device){ //创建管线布局的方法
    NUM_DESCRIPTOR_SETS=1; //设置描述集数量
    VkDescriptorSetLayoutBinding layout_bindings[2]; //描述集布局绑定数组
    layout_bindings[0].binding = 0; //此绑定的绑定点编号为 0
    layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //描述类型
    layout_bindings[0].descriptorCount = 1; //描述数量
    layout_bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; //目标着色器阶段
    layout_bindings[0].pImmutableSamplers = NULL;
    layout_bindings[1].binding = 1; //此绑定的绑定点编号为 1
    layout_bindings[1].descriptorType =VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layout_bindings[1].descriptorCount = 1; //描述数量
    layout_bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; //目标着色器阶段
    layout_bindings[1].pImmutableSamplers = NULL;
    VkDescriptorSetLayoutCreateInfo descriptor_layout = {}; //构建描述集布局创建信息结构体实例
    descriptor_layout.sType =VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO; //结构体类型
    descriptor_layout.pNext = NULL; //自定义数据的指针
    descriptor_layout.bindingCount = 2; //描述集布局绑定的数量
    descriptor_layout.pBindings = layout_bindings; //描述集布局绑定数组
    descLayouts.resize(NUM_DESCRIPTOR_SETS); //调整描述集布局列表尺寸
    VkResult result = vk::vkCreateDescriptorSetLayout(device,&descriptor_layout, NULL, descLayouts.data()); //创建描述集布局
    assert(result == VK_SUCCESS); //检查描述集布局创建是否成功
    const unsigned push_constant_range_count = 1; //推送常量范围实例数量
    VkPushConstantRange push_constant_ranges[push_constant_range_count] = {};//推送常量范围数组
    push_constant_ranges[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT; //着色器阶段
    push_constant_ranges[0].offset = 0; //起始偏移量
    push_constant_ranges[0].size = sizeof(float)*16; //数据字节数
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {}; //构建管线布局创建信息结构体实例
    pPipelineLayoutCreateInfo.sType =VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO; //结构体类型
    pPipelineLayoutCreateInfo.pNext = NULL; //自定义数据的指针
    pPipelineLayoutCreateInfo.pushConstantRangeCount =
            push_constant_range_count; //推送常量范围的数量
    pPipelineLayoutCreateInfo.pPushConstantRanges = push_constant_ranges; //推送常量范围的列表
    pPipelineLayoutCreateInfo.setLayoutCount = NUM_DESCRIPTOR_SETS; //描述集布局的数量
    pPipelineLayoutCreateInfo.pSetLayouts = descLayouts.data(); //描述集布局列表
    result = vk::vkCreatePipelineLayout(device,&pPipelineLayoutCreateInfo, NULL, &pipelineLayout); //创建管线布局
    assert(result == VK_SUCCESS); //检查创建是否成功
}
void ShaderQueueSuit_CommonTex::destroy_pipeline_layout(VkDevice& device)
{
    for (int i = 0; i < NUM_DESCRIPTOR_SETS; i++)
    {
        vk::vkDestroyDescriptorSetLayout(device, descLayouts[i], NULL);
    }
    vk::vkDestroyPipelineLayout(device, pipelineLayout, NULL);
}
void ShaderQueueSuit_CommonTex::init_descriptor_set(VkDevice& device){ //初始化描述集的方法
    VkDescriptorPoolSize type_count[2]; //描述集池尺寸实例数组
    type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //第 1 个描述类型
    type_count[0].descriptorCount = TextureManager::texNames.size(); //第 1 个描述数量
    type_count[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //第 2 个描述类型
    type_count[1].descriptorCount = TextureManager::texNames.size(); //第 2 个描述数量
    VkDescriptorPoolCreateInfo descriptor_pool = {}; //构建描述集池创建信息结构体实例
    descriptor_pool.sType =VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO; //结构体类型
    descriptor_pool.pNext = NULL; //自定义数据的指针
    descriptor_pool.maxSets = TextureManager::texNames.size(); //描述集最大数量
    descriptor_pool.poolSizeCount = 2; //描述集池尺寸实例数量
    descriptor_pool.pPoolSizes = type_count; //描述集池尺寸实例数组
    VkResult result = vk::vkCreateDescriptorPool(device, &descriptor_pool, NULL, &descPool); //创建描述集池
    assert(result == VK_SUCCESS); //检查描述集池创建是否成功
    std::vector<VkDescriptorSetLayout> layouts; //描述集布局列表
    for(int i=0;i<TextureManager::texNames.size();i++){ //遍历所有纹理
        layouts.push_back(descLayouts[0]); //向列表中添加指定描述集布局
    }
    VkDescriptorSetAllocateInfo alloc_info[1]; //构建描述集分配信息结构体实例数组
    alloc_info[0].sType =VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO; //结构体类型
    alloc_info[0].pNext = NULL; //自定义数据的指针
    alloc_info[0].descriptorPool = descPool; //指定描述集池
    alloc_info[0].descriptorSetCount = TextureManager::texNames.size(); //描述集数量
    alloc_info[0].pSetLayouts = layouts.data(); //描述集布局列表
    descSet.resize(TextureManager::texNames.size()); //调整描述集列表尺寸
    result = vk::vkAllocateDescriptorSets(device,  alloc_info, descSet.data());//分配指定数量的描述集
    assert(result == VK_SUCCESS); //检查描述集分配是否成功
    writes[0] = {}; //完善一致变量写入描述集实例数组元素 0
    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET; //结构体类型
    writes[0].pNext = NULL; //自定义数据的指针
    writes[0].descriptorCount = 1; //描述数量
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //描述类型(一致变量缓冲)
    writes[0].pBufferInfo = &uniformBufferInfo; //对应一致变量缓冲的信息
    writes[0].dstArrayElement = 0; //目标数组起始元素
    writes[0].dstBinding = 0; //目标绑定编号
    writes[1] = {}; //完善一致变量写入描述集实例数组元素 1
    writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET; //结构体类型
    writes[1].dstBinding = 1; //目标绑定编号
    writes[1].descriptorCount = 1; //描述数量
    writes[1].descriptorType =VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //描述类型（采样用纹理）
    writes[1].dstArrayElement = 0; //目标数组起始元素
}
void ShaderQueueSuit_CommonTex::create_shader(VkDevice& device)
{
    std::string vertStr= FileUtil::loadAssetStr("shader/commonTex.vert");
    std::string fragStr= FileUtil::loadAssetStr("shader/commonTex.frag");
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].pNext = NULL;
    shaderStages[0].pSpecializationInfo = NULL;
    shaderStages[0].flags = 0;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].pName = "main";
    std::vector<unsigned int> vtx_spv;
    bool retVal = GLSLtoSPV(VK_SHADER_STAGE_VERTEX_BIT, vertStr.c_str(), vtx_spv);
    assert(retVal);
    LOGE("顶点着色器脚本编译SPV成功！");
    VkShaderModuleCreateInfo moduleCreateInfo;
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);
    moduleCreateInfo.pCode = vtx_spv.data();
    VkResult result = vk::vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderStages[0].module);
    assert(result == VK_SUCCESS);
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].pNext = NULL;
    shaderStages[1].pSpecializationInfo = NULL;
    shaderStages[1].flags = 0;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].pName = "main";
    std::vector<unsigned int> frag_spv;
    retVal = GLSLtoSPV(VK_SHADER_STAGE_FRAGMENT_BIT, fragStr.c_str(), frag_spv);
    assert(retVal);
    LOGE("片元着色器脚本编译SPV成功！");
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = frag_spv.size() * sizeof(unsigned int);
    moduleCreateInfo.pCode = frag_spv.data();
    result = vk::vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderStages[1].module);
    assert(result == VK_SUCCESS);
}
void ShaderQueueSuit_CommonTex::destroy_shader(VkDevice& device)
{
    vk::vkDestroyShaderModule(device, shaderStages[0].module, NULL);
    vk::vkDestroyShaderModule(device, shaderStages[1].module, NULL);
}
void ShaderQueueSuit_CommonTex::initVertexAttributeInfo(){ //设置顶点着色器输入属性信息
    vertexBinding.binding = 0; //对应绑定点
    vertexBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; //数据输入频率为每顶点
    vertexBinding.stride = sizeof(float)*5; //每组数据的跨度字节数
    vertexAttribs[0].binding = 0; //第 1 个顶点输入属性的绑定点
    vertexAttribs[0].location = 0; //第 1 个顶点输入属性的位置索引
    vertexAttribs[0].format = VK_FORMAT_R32G32B32_SFLOAT; //第 1 个顶点输入属性的数据格式
    vertexAttribs[0].offset = 0; //第 1 个顶点输入属性的偏移量
    vertexAttribs[1].binding = 0; //第 2 个顶点输入属性的绑定点
    vertexAttribs[1].location = 1; //第 2 个顶点输入属性的位置索引
    vertexAttribs[1].format = VK_FORMAT_R32G32_SFLOAT; //第 2 个顶点输入属性的数据格式
    vertexAttribs[1].offset = 12; //第 2 个顶点输入属性的偏移量
}
void ShaderQueueSuit_CommonTex::create_pipe_line(VkDevice& device,VkRenderPass& renderPass)
{
    VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
    memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = NULL;
    dynamicState.pDynamicStates = dynamicStateEnables;
    dynamicState.dynamicStateCount = 0;
    VkPipelineVertexInputStateCreateInfo vi;
    vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vi.pNext = NULL;
    vi.flags = 0;
    vi.vertexBindingDescriptionCount = 1;
    vi.pVertexBindingDescriptions = &vertexBinding;
    vi.vertexAttributeDescriptionCount = 2;
    vi.pVertexAttributeDescriptions =vertexAttribs;
    VkPipelineInputAssemblyStateCreateInfo ia;
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.pNext = NULL;
    ia.flags = 0;
    ia.primitiveRestartEnable = VK_FALSE;
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    VkPipelineRasterizationStateCreateInfo rs;
    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.pNext = NULL;
    rs.flags = 0;
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.cullMode = VK_CULL_MODE_NONE;
    rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rs.depthClampEnable = VK_TRUE;
    rs.rasterizerDiscardEnable = VK_FALSE;
    rs.depthBiasEnable = VK_FALSE;
    rs.depthBiasConstantFactor = 0;
    rs.depthBiasClamp = 0;
    rs.depthBiasSlopeFactor = 0;
    rs.lineWidth = 1.0f;
    VkPipelineColorBlendAttachmentState att_state[1];
    att_state[0].colorWriteMask = 0xf;
    att_state[0].blendEnable = VK_TRUE;
    att_state[0].alphaBlendOp = VK_BLEND_OP_ADD;
    att_state[0].colorBlendOp = VK_BLEND_OP_ADD;
    att_state[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    att_state[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    att_state[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    att_state[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
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
    VkViewport viewports;
    viewports.minDepth = 0.0f;
    viewports.maxDepth = 1.0f;
    viewports.x = 0;
    viewports.y = 0;
    viewports.width = MyVulkanManager::screenWidth;
    viewports.height = MyVulkanManager::screenHeight;
    VkRect2D scissor;
    scissor.extent.width = MyVulkanManager::screenWidth;
    scissor.extent.height = MyVulkanManager::screenHeight;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    VkPipelineViewportStateCreateInfo vp = {};
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.pNext = NULL;
    vp.flags = 0;
    vp.viewportCount = 1;
    vp.scissorCount = 1;
    vp.pScissors = &scissor;
    vp.pViewports = &viewports;
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
    VkPipelineCacheCreateInfo pipelineCacheInfo;
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCacheInfo.pNext = NULL;
    pipelineCacheInfo.initialDataSize = 0;
    pipelineCacheInfo.pInitialData = NULL;
    pipelineCacheInfo.flags = 0;
    VkResult result = vk::vkCreatePipelineCache(device, &pipelineCacheInfo, NULL,&pipelineCache);
    assert(result == VK_SUCCESS);
    result = vk::vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineInfo, NULL, &pipeline);
    assert(result == VK_SUCCESS);
}
void ShaderQueueSuit_CommonTex::destroy_pipe_line(VkDevice& device)
{
    vk::vkDestroyPipeline(device, pipeline, NULL);
    vk::vkDestroyPipelineCache(device, pipelineCache, NULL);
}
ShaderQueueSuit_CommonTex::ShaderQueueSuit_CommonTex(VkDevice* deviceIn,VkRenderPass& renderPass,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    this->devicePointer=deviceIn;
    create_uniform_buffer(*devicePointer,memoryroperties);
    create_pipeline_layout(*devicePointer);
    init_descriptor_set(*devicePointer);
    create_shader(*devicePointer);
    initVertexAttributeInfo();
    create_pipe_line(*devicePointer,renderPass);
}
ShaderQueueSuit_CommonTex::~ShaderQueueSuit_CommonTex()
{
    destroy_pipe_line(*devicePointer);
    destroy_shader(*devicePointer);
    destroy_pipeline_layout(*devicePointer);
    destroy_uniform_buffer(*devicePointer);
}