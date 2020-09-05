#include "ShaderQueueSuit_Common.h"
#include <assert.h>
#include "HelpFunction.h"
#include "mylog.h"
#include "../util/FileUtil.h"
#include "MyVulkanManager.h"
#include "ShaderCompileUtil.h"

//创建一致变量缓冲
void ShaderQueueSuit_Common::create_uniform_buffer(VkDevice &device,
                                                   VkPhysicalDeviceMemoryProperties &memoryroperties) {
    bufferByteCount = sizeof(float) * 16;//一致变量缓冲的总字节数

    VkBufferCreateInfo buf_info = {};//构建一致变量缓冲创建信息结构体实例
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;    //结构体的类型
    buf_info.pNext = NULL;//自定义数据的指针
    buf_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;//缓冲的用途
    buf_info.size = bufferByteCount;//缓冲总字节数
    buf_info.queueFamilyIndexCount = 0;    //队列家族数量
    buf_info.pQueueFamilyIndices = NULL;//队列家族索引列表
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;//共享模式
    buf_info.flags = 0;//标志

    VkResult result = vk::vkCreateBuffer(device, &buf_info, NULL, &uniformBuf);//创建一致变量缓冲
    assert(result == VK_SUCCESS);//检查创建是否成功

    VkMemoryRequirements mem_reqs;//内存需求变量
    vk::vkGetBufferMemoryRequirements(device, uniformBuf, &mem_reqs);//获取此缓冲的内存需求

    VkMemoryAllocateInfo alloc_info = {};//构建内存分配信息结构体实例
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;//结构体类型
    alloc_info.pNext = NULL;//自定义数据的指针
    alloc_info.memoryTypeIndex = 0;//内存类型索引
    alloc_info.allocationSize = mem_reqs.size;//缓冲内存分配字节数

    VkFlags requirements_mask =
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;//需要的内存类型掩码
    bool flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits,
                                         requirements_mask,
                                         &alloc_info.memoryTypeIndex);    //获取所需内存类型索引
    if (flag) { LOGE("确定内存类型成功 类型索引为%d", alloc_info.memoryTypeIndex); }
    else { LOGE("确定内存类型失败!"); }

    result = vk::vkAllocateMemory(device, &alloc_info, NULL, &memUniformBuf);//分配内存
    assert(result == VK_SUCCESS);//检查内存分配是否成功
    result = vk::vkBindBufferMemory(device, uniformBuf, memUniformBuf, 0);//将内存和对应缓冲绑定
    assert(result == VK_SUCCESS);//检查绑定操作是否成功

    uniformBufferInfo.buffer = uniformBuf;//指定一致变量缓冲
    uniformBufferInfo.offset = 0;//起始偏移量
    uniformBufferInfo.range = bufferByteCount;//一致变量缓冲总字节数

}

void ShaderQueueSuit_Common::destroy_uniform_buffer(VkDevice &device)//销毁一致变量缓冲相关
{
    vk::vkDestroyBuffer(device, uniformBuf, NULL);//销毁一致变量缓冲
    vk::vkFreeMemory(device, memUniformBuf, NULL);//释放一致变量缓冲对应设备内存
}

//创建管线布局
void ShaderQueueSuit_Common::create_pipeline_layout(VkDevice &device) {
    NUM_DESCRIPTOR_SETS = 1;//设置描述集数量

    VkDescriptorSetLayoutBinding layout_bindings[1];//描述集布局绑定数组
    layout_bindings[0].binding = 0;//此绑定的绑定点编号
    layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//描述类型
    layout_bindings[0].descriptorCount = 1;//描述数量
    layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;    //目标着色器阶段
    layout_bindings[0].pImmutableSamplers = NULL;

    VkDescriptorSetLayoutCreateInfo descriptor_layout = {};    //构建描述集布局创建信息结构体实例
    descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;//结构体类型
    descriptor_layout.pNext = NULL;//自定义数据的指针
    descriptor_layout.bindingCount = 1;//描述集布局绑定的数量
    descriptor_layout.pBindings = layout_bindings;//描述集布局绑定数组

    descLayouts.resize(NUM_DESCRIPTOR_SETS);//调整描述集布局列表尺寸
    VkResult result = vk::vkCreateDescriptorSetLayout(device, &descriptor_layout, NULL,
                                                      descLayouts.data());//创建描述集布局
    assert(result == VK_SUCCESS);//检查描述集布局创建是否成功

    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};//构建管线布局创建信息结构体实例
    pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;    //结构体类型
    pPipelineLayoutCreateInfo.pNext = NULL;//自定义数据的指针
    pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;//推送常量范围的数量
    pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;//推送常量范围的列表
    pPipelineLayoutCreateInfo.setLayoutCount = NUM_DESCRIPTOR_SETS;//描述集布局的数量
    pPipelineLayoutCreateInfo.pSetLayouts = descLayouts.data();//描述集布局列表

    result = vk::vkCreatePipelineLayout(device, &pPipelineLayoutCreateInfo, NULL,
                                        &pipelineLayout);//创建管线布局
    assert(result == VK_SUCCESS);//检查创建是否成功
}

void ShaderQueueSuit_Common::destroy_pipeline_layout(VkDevice &device)//销毁管线布局的方法
{
    for (int i = 0; i < NUM_DESCRIPTOR_SETS; i++)//遍历描述集列表
    {
        vk::vkDestroyDescriptorSetLayout(device, descLayouts[i], NULL);//销毁对应描述集布局
    }
    vk::vkDestroyPipelineLayout(device, pipelineLayout, NULL);//销毁管线布局
}

//初始化描述集
void ShaderQueueSuit_Common::init_descriptor_set(VkDevice &device) {
    VkDescriptorPoolSize type_count[1];//描述集池尺寸实例数组
    type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//描述类型
    type_count[0].descriptorCount = 1;//描述数量

    VkDescriptorPoolCreateInfo descriptor_pool = {};//构建描述集池创建信息结构体实例
    descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;//结构体类型
    descriptor_pool.pNext = NULL;//自定义数据的指针
    descriptor_pool.maxSets = 1;//描述集最大数量
    descriptor_pool.poolSizeCount = 1;//描述集池尺寸实例数量
    descriptor_pool.pPoolSizes = type_count;//描述集池尺寸实例数组
    VkResult result = vk::vkCreateDescriptorPool(device, &descriptor_pool, NULL, &descPool);//创建描述集池
    assert(result == VK_SUCCESS);//检查描述集池创建是否成功

    std::vector<VkDescriptorSetLayout> layouts;//描述集布局列表
    layouts.push_back(descLayouts[0]);//向列表中添加指定描述集布局

    VkDescriptorSetAllocateInfo alloc_info[1];//构建描述集分配信息结构体实例数组
    alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;//结构体类型
    alloc_info[0].pNext = NULL;//自定义数据的指针
    alloc_info[0].descriptorPool = descPool;//指定描述集池
    alloc_info[0].descriptorSetCount = 1;//描述集数量
    alloc_info[0].pSetLayouts = layouts.data();//描述集布局列表
    descSet.resize(1);//调整描述集列表尺寸
    result = vk::vkAllocateDescriptorSets(device, alloc_info, descSet.data());//分配描述集
    assert(result == VK_SUCCESS);//检查描述集分配是否成功

    writes[0] = {}; //完善一致变量写入描述集实例数组
    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;//结构体类型
    writes[0].pNext = NULL;    //自定义数据的指针
    writes[0].descriptorCount = 1;//描述数量
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//描述类型
    writes[0].pBufferInfo = &uniformBufferInfo;//对应一致变量缓冲的信息
    writes[0].dstArrayElement = 0;//目标数组起始元素
    writes[0].dstBinding = 0;//目标绑定编号

}

void ShaderQueueSuit_Common::create_shader(VkDevice &device) {
    std::string vertStr = FileUtil::loadAssetStr("shader/commonTexLight.vert");//加载顶点着色器脚本
    std::string fragStr = FileUtil::loadAssetStr("shader/commonTexLight.frag");//加载片元着色器脚本

    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].pNext = NULL;//自定义数据的指针
    shaderStages[0].pSpecializationInfo = NULL;//特殊信息
    shaderStages[0].flags = 0;//供将来使用的标志
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;    //着色器阶段为顶点
    shaderStages[0].pName = "main";//入口函数为main

    std::vector<unsigned int> vtx_spv;    //将顶点着色器脚本编译为SPV
    bool retVal = GLSLtoSPV(VK_SHADER_STAGE_VERTEX_BIT, vertStr.c_str(), vtx_spv);
    assert(retVal);//检查编译是否成功
    LOGE("顶点着色器脚本编译SPV成功！");

    VkShaderModuleCreateInfo moduleCreateInfo;//准备顶点着色器模块创建信息
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;//自定义数据的指针
    moduleCreateInfo.flags = 0;//供将来使用的标志
    moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);//顶点着色器SPV数据总字节数
    moduleCreateInfo.pCode = vtx_spv.data();//顶点着色器SPV数据
    VkResult result = vk::vkCreateShaderModule(device, &moduleCreateInfo, NULL,
                                               &shaderStages[0].module);//创建顶点着色器模块
    assert(result == VK_SUCCESS);//检查顶点着色器模块创建是否成功

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;//结构体类型
    shaderStages[1].pNext = NULL;//自定义数据的指针
    shaderStages[1].pSpecializationInfo = NULL;//特殊信息
    shaderStages[1].flags = 0;//供将来使用的标志
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;//着色器阶段为片元
    shaderStages[1].pName = "main";//入口函数为main

    std::vector<unsigned int> frag_spv;
    retVal = GLSLtoSPV(VK_SHADER_STAGE_FRAGMENT_BIT, fragStr.c_str(), frag_spv); //将片元着色器脚本编译为SPV
    assert(retVal);//检查编译是否成功
    LOGE("片元着色器脚本编译SPV成功！");
    //准备片元着色器模块创建信息
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;//设置结构体类型
    moduleCreateInfo.pNext = NULL;//自定义数据的指针
    moduleCreateInfo.flags = 0;//供将来使用的标志
    moduleCreateInfo.codeSize = frag_spv.size() * sizeof(unsigned int);//片元着色器SPV数据总字节数
    moduleCreateInfo.pCode = frag_spv.data();//片元着色器SPV数据
    result = vk::vkCreateShaderModule(device, &moduleCreateInfo, NULL,
                                      &shaderStages[1].module);//创建片元着色器模块
    assert(result == VK_SUCCESS);//检查片元着色器模块创建是否成功

}

void ShaderQueueSuit_Common::destroy_shader(VkDevice &device)//销毁着色器模块的方法
{
    vk::vkDestroyShaderModule(device, shaderStages[0].module, NULL);//销毁顶点着色器模块
    vk::vkDestroyShaderModule(device, shaderStages[1].module, NULL);//销毁片元着色器模块
}

void ShaderQueueSuit_Common::initVertexAttributeInfo() {
    vertexBinding.binding = 0;//对应绑定点
    vertexBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;    //数据输入频率为每顶点
    vertexBinding.stride = sizeof(float) * 6;//每组数据的跨度字节数

    vertexAttribs[0].binding = 0;//第1个顶点输入属性的绑定点
    vertexAttribs[0].location = 0;//第1个顶点输入属性的位置索引
    vertexAttribs[0].format = VK_FORMAT_R32G32B32_SFLOAT;//第1个顶点输入属性的数据格式
    vertexAttribs[0].offset = 0;//第1个顶点输入属性的偏移量

    vertexAttribs[1].binding = 0;//第2个顶点输入属性的绑定点
    vertexAttribs[1].location = 1;//第2个顶点输入属性的位置索引
    vertexAttribs[1].format = VK_FORMAT_R32G32B32_SFLOAT;//第2个顶点输入属性的数据格式
    vertexAttribs[1].offset = 12;//第2个顶点输入属性的偏移量

}

void ShaderQueueSuit_Common::create_pipe_line(VkDevice &device, VkRenderPass &renderPass) {
    VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];//动态状态启用标志
    memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);    //设置所有标志为false

    VkPipelineDynamicStateCreateInfo dynamicState = {};//管线动态状态创建信息
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;//结构体类型
    dynamicState.pNext = NULL;//自定义数据的指针
    dynamicState.pDynamicStates = dynamicStateEnables;//动态状态启用标志数组
    dynamicState.dynamicStateCount = 0;//启用的动态状态项数量

    VkPipelineVertexInputStateCreateInfo vi;//管线顶点数据输入状态创建信息
    vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vi.pNext = NULL;//自定义数据的指针
    vi.flags = 0;//供将来使用的标志
    vi.vertexBindingDescriptionCount = 1;//顶点输入绑定描述数量
    vi.pVertexBindingDescriptions = &vertexBinding;//顶点输入绑定描述列表
    vi.vertexAttributeDescriptionCount = 2;//顶点输入属性数量
    vi.pVertexAttributeDescriptions = vertexAttribs;//顶点输入属性描述列表

    VkPipelineInputAssemblyStateCreateInfo ia;//管线图元组装状态创建信息
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.pNext = NULL;//自定义数据的指针
    ia.flags = 0;//供将来使用的标志
    ia.primitiveRestartEnable = VK_FALSE;//关闭图元重启
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;    //采用三角形图元列表模式

    VkPipelineRasterizationStateCreateInfo rs;//管线光栅化状态创建信息
    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.pNext = NULL;//自定义数据的指针
    rs.flags = 0;//供将来使用的标志
    rs.polygonMode = VK_POLYGON_MODE_FILL;//绘制方式为填充
    rs.cullMode = VK_CULL_MODE_NONE;//不使用背面剪裁
    rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;    //卷绕方向为逆时针
    rs.depthClampEnable = VK_TRUE;//深度截取
    rs.rasterizerDiscardEnable = VK_FALSE;//启用光栅化操作（若为TRUE则光栅化不产生任何片元）
    rs.depthBiasEnable = VK_FALSE;//不启用深度偏移
    rs.depthBiasConstantFactor = 0;    //深度偏移常量因子
    rs.depthBiasClamp = 0;//深度偏移值上下限（若为正作为上限，为负作为下限）
    rs.depthBiasSlopeFactor = 0;//深度偏移斜率因子
    rs.lineWidth = 1.0f;//线宽度（仅在线绘制模式起作用）

    VkPipelineColorBlendAttachmentState att_state[1];//管线颜色混合附件状态数组
    att_state[0].colorWriteMask = 0xf;//设置写入掩码
    att_state[0].blendEnable = VK_FALSE;//关闭混合
    att_state[0].alphaBlendOp = VK_BLEND_OP_ADD;//设置Alpha通道混合方式
    att_state[0].colorBlendOp = VK_BLEND_OP_ADD;//设置RGB通道混合方式
    att_state[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;//设置源颜色混合因子
    att_state[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;//设置目标颜色混合因子
    att_state[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;//设置源Alpha混合因子
    att_state[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;//设置目标Alpha混合因子


    VkPipelineColorBlendStateCreateInfo cb;//管线的颜色混合状态创建信息
    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    cb.pNext = NULL;//自定义数据的指针
    cb.flags = 0;//供未来使用的标志
    cb.attachmentCount = 1;    //颜色混合附件数量
    cb.pAttachments = att_state;//颜色混合附件列表
    cb.logicOpEnable = VK_FALSE;//不启用逻辑操作
    cb.logicOp = VK_LOGIC_OP_NO_OP;//逻辑操作类型为无
    cb.blendConstants[0] = 1.0f;//混合常量R分量
    cb.blendConstants[1] = 1.0f;//混合常量G分量
    cb.blendConstants[2] = 1.0f;//混合常量B分量
    cb.blendConstants[3] = 1.0f;//混合常量A分量

    VkViewport viewports;//视口信息
    viewports.minDepth = 0.0f;//视口最小深度
    viewports.maxDepth = 1.0f;//视口最大深度
    viewports.x = 0;//视口X坐标
    viewports.y = 0;//视口Y坐标
    viewports.width = MyVulkanManager::screenWidth;//视口宽度
    viewports.height = MyVulkanManager::screenHeight;//视口高度

    VkRect2D scissor;//剪裁窗口信息
    scissor.extent.width = MyVulkanManager::screenWidth;//剪裁窗口的宽度
    scissor.extent.height = MyVulkanManager::screenHeight;//剪裁窗口的高度
    scissor.offset.x = 0;//剪裁窗口的X坐标
    scissor.offset.y = 0;//剪裁窗口的Y坐标

    VkPipelineViewportStateCreateInfo vp = {};//管线视口状态创建信息
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.pNext = NULL;//自定义数据的指针
    vp.flags = 0;//供将来使用的标志
    vp.viewportCount = 1;//视口的数量
    vp.scissorCount = 1;//剪裁窗口的数量
    vp.pScissors = &scissor;//剪裁窗口信息列表
    vp.pViewports = &viewports;//视口信息列表

    VkPipelineDepthStencilStateCreateInfo ds;//管线深度及模板状态创建信息
    ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    ds.pNext = NULL;//自定义数据的指针
    ds.flags = 0;//供将来使用的标志
    ds.depthTestEnable = VK_TRUE;//开启深度测试
    ds.depthWriteEnable = VK_TRUE;//开启深度值写入
    ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;//深度检测比较操作
    ds.depthBoundsTestEnable = VK_FALSE;//关闭深度边界测试
    ds.minDepthBounds = 0;//最小深度边界
    ds.maxDepthBounds = 0;//最大深度边界
    ds.stencilTestEnable = VK_FALSE;//关闭模板测试
    ds.back.failOp = VK_STENCIL_OP_KEEP;//未通过模板测试时的操作
    ds.back.passOp = VK_STENCIL_OP_KEEP;//模板测试、深度测试都通过时的操作
    ds.back.compareOp = VK_COMPARE_OP_ALWAYS;//模板测试的比较操作
    ds.back.compareMask = 0;//模板测试比较掩码
    ds.back.reference = 0;//模板测试参考值
    ds.back.depthFailOp = VK_STENCIL_OP_KEEP;//未通过深度测试时的操作
    ds.back.writeMask = 0;//写入掩码
    ds.front = ds.back;

    VkPipelineMultisampleStateCreateInfo ms;//管线多重采样状态创建信息
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.pNext = NULL;//自定义数据的指针
    ms.flags = 0;//供将来使用的标志位
    ms.pSampleMask = NULL;//采样掩码
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;//光栅化阶段采样数量
    ms.sampleShadingEnable = VK_FALSE;//关闭采样着色
    ms.alphaToCoverageEnable = VK_FALSE;//不启用alphaToCoverage
    ms.alphaToOneEnable = VK_FALSE;//不启用alphaToOne
    ms.minSampleShading = 0.0;//最小采样着色

    VkGraphicsPipelineCreateInfo pipelineInfo;//图形管线创建信息
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = NULL;//自定义数据的指针
    pipelineInfo.layout = pipelineLayout;//指定管线布局
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;//基管线句柄
    pipelineInfo.basePipelineIndex = 0;//基管线索引
    pipelineInfo.flags = 0;    //标志
    pipelineInfo.pVertexInputState = &vi;//管线的顶点数据输入状态信息
    pipelineInfo.pInputAssemblyState = &ia;//管线的图元组装状态信息
    pipelineInfo.pRasterizationState = &rs;//管线的光栅化状态信息
    pipelineInfo.pColorBlendState = &cb;//管线的颜色混合状态信息
    pipelineInfo.pTessellationState = NULL;//管线的曲面细分状态信息
    pipelineInfo.pMultisampleState = &ms;//管线的多重采样状态信息
    pipelineInfo.pDynamicState = &dynamicState;//管线的动态状态信息
    pipelineInfo.pViewportState = &vp;//管线的视口状态信息
    pipelineInfo.pDepthStencilState = &ds; //管线的深度模板测试状态信息
    pipelineInfo.stageCount = 2;//管线的着色阶段数量
    pipelineInfo.pStages = shaderStages;//管线的着色阶段列表
    pipelineInfo.renderPass = renderPass;//指定的渲染通道
    pipelineInfo.subpass = 0;//设置管线执行对应的渲染子通道

    VkPipelineCacheCreateInfo pipelineCacheInfo;//管线缓冲创建信息
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCacheInfo.pNext = NULL;//自定义数据的指针
    pipelineCacheInfo.initialDataSize = 0;//初始数据尺寸
    pipelineCacheInfo.pInitialData = NULL;//初始数据内容，此处为NULL
    pipelineCacheInfo.flags = 0;//供将来使用的标志位

    VkResult result = vk::vkCreatePipelineCache(device, &pipelineCacheInfo, NULL,
                                                &pipelineCache);//创建管线缓冲
    assert(result == VK_SUCCESS);//检查管线缓冲创建是否成功
    result = vk::vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineInfo, NULL,
                                           &pipeline);//创建管线
    assert(result == VK_SUCCESS);//检查管线创建是否成功

}

void ShaderQueueSuit_Common::destroy_pipe_line(VkDevice &device) {
    //销毁管线
    vk::vkDestroyPipeline(device, pipeline, NULL);
    //销毁管线缓冲
    vk::vkDestroyPipelineCache(device, pipelineCache, NULL);
}

ShaderQueueSuit_Common::ShaderQueueSuit_Common(VkDevice *deviceIn, VkRenderPass &renderPass,
                                               VkPhysicalDeviceMemoryProperties &memoryroperties) {
    this->devicePointer = deviceIn;
    create_uniform_buffer(*devicePointer, memoryroperties);//创建一致变量缓冲
    create_pipeline_layout(*devicePointer);//创建管线布局
    init_descriptor_set(*devicePointer);//初始化描述集
    create_shader(*devicePointer);//创建着色器
    initVertexAttributeInfo();//初始化顶点属性信息
    create_pipe_line(*devicePointer, renderPass);//创建管线

}

ShaderQueueSuit_Common::~ShaderQueueSuit_Common() {//析构函数
    destroy_pipe_line(*devicePointer);//销毁管线
    destroy_shader(*devicePointer);//销毁着色器模块
    destroy_pipeline_layout(*devicePointer);//销毁管线布局
    destroy_uniform_buffer(*devicePointer);//销毁一致变量缓冲
}
