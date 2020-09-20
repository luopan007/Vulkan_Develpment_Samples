#include "DrawableObjectCommon.h"
#include "mylog.h"
#include <assert.h>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include <string.h>
DrawableObjectCommonLight::DrawableObjectCommonLight(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    pushConstantData=new float[16];					
    this->devicePointer=&device;
    this->vdata=vdataIn;
    this->vCount=vCountIn;
    VkBufferCreateInfo buf_info = {};
    buf_info.sType =VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.size = dataByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(device,&buf_info, NULL, &vertexDatabuf);
    assert(result == VK_SUCCESS);
    VkMemoryRequirements mem_reqs;
    vk::vkGetBufferMemoryRequirements(device, vertexDatabuf, &mem_reqs);
    assert(dataByteCount<=mem_reqs.size);
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = mem_reqs.size;
    VkFlags requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bool flag=memoryTypeFromProperties(memoryroperties,mem_reqs.memoryTypeBits,requirements_mask,&alloc_info.memoryTypeIndex);
    if(flag){
        LOGE("确定内存类型成功 类型索引为%d",alloc_info.memoryTypeIndex);
    }else{
        LOGE("确定内存类型失败!");
    }
    result = vk::vkAllocateMemory(device,&alloc_info, NULL, &vertexDataMem);
    assert(result == VK_SUCCESS);
    uint8_t *pData;
    result = vk::vkMapMemory(device, vertexDataMem, 0, mem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, vdata, dataByteCount);
    vk::vkUnmapMemory(device, vertexDataMem);
    result = vk::vkBindBufferMemory(device,vertexDatabuf, vertexDataMem, 0);
    assert(result == VK_SUCCESS);
    vertexDataBufferInfo.buffer = vertexDatabuf;
    vertexDataBufferInfo.offset = 0;
    vertexDataBufferInfo.range = mem_reqs.size;
    initDrawCmdbuf(device, memoryroperties);
}
void DrawableObjectCommonLight::initDrawCmdbuf( //用于创建间接绘制信息数据缓冲的方法
        VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryroperties){
    indirectDrawCount = 1; //间接绘制信息数据组的数量
    drawCmdbufbytes= indirectDrawCount*sizeof(VkDrawIndirectCommand); //信息数据所占总字节数
    VkBufferCreateInfo buf_info = {}; //构建缓冲创建信息结构体实例
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO; //设置结构体类型
    buf_info.pNext = NULL; //自定义数据的指针
    buf_info.usage = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT; //设置缓冲用途
    buf_info.size = drawCmdbufbytes; //设置数据总字节数
    buf_info.queueFamilyIndexCount = 0; //队列家族数量
    buf_info.pQueueFamilyIndices = NULL; //队列家族列表
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //共享模式
    buf_info.flags = 0; //标志
    VkResult result = vk::vkCreateBuffer(device, &buf_info, NULL, &drawCmdbuf); //创建缓冲
    assert(result == VK_SUCCESS); //检查创建缓冲是否成功
    VkMemoryRequirements mem_reqs; //缓冲内存需求
    vk::vkGetBufferMemoryRequirements(device, drawCmdbuf, &mem_reqs); //获取缓冲内存需求
    assert(drawCmdbufbytes <= mem_reqs.size); //检查内存需求获取是否正确
    VkMemoryAllocateInfo alloc_info = {}; //构建内存分配信息结构体实例
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;//设置结构体类型
    alloc_info.pNext = NULL; //自定义数据的指针
    alloc_info.memoryTypeIndex = 0; //内存类型索引
    alloc_info.allocationSize = mem_reqs.size; //内存总字节数
    VkFlags requirements_mask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT; //需要的内存类型掩码
    bool flag = memoryTypeFromProperties(memoryroperties, //获取所需内存类型索引
                                         mem_reqs.memoryTypeBits, requirements_mask, &alloc_info.memoryTypeIndex);
    if (flag){
        LOGE("确定内存类型成功 类型索引为%d", alloc_info.memoryTypeIndex);
    }else{
        LOGE("确定内存类型失败!");
    }
    result = vk::vkAllocateMemory(device, &alloc_info, NULL, &drawCmdMem);//为缓冲分配内存
    assert(result == VK_SUCCESS); //检查内存分配是否成功
    uint8_t *pData; // CPU 访问时的辅助指针
    result = vk::vkMapMemory(device, //将设备内存映射为CPU 可访问
                             drawCmdMem, 0, mem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS); //检查映射是否成功
    VkDrawIndirectCommand dic; //构建间接绘制信息结构体实例
    dic.vertexCount = vCount; //顶点数量
    dic.firstInstance = 0; //第一个绘制的实例序号
    dic.firstVertex = 0; //第一个绘制用的顶点索引
    dic.instanceCount = 1; //需要绘制的实例数量
    memcpy(pData, &dic, drawCmdbufbytes); //将数据拷贝进设备内存
    vk::vkUnmapMemory(device, vertexDataMem); //解除内存映射
    result = vk::vkBindBufferMemory(device, drawCmdbuf, drawCmdMem, 0); //绑定内存与缓冲
    assert(result == VK_SUCCESS); //检查绑定是否成功
}


DrawableObjectCommonLight::~DrawableObjectCommonLight()
{
    delete vdata;
    vk::vkDestroyBuffer(*devicePointer, vertexDatabuf, NULL);
    vk::vkFreeMemory(*devicePointer, vertexDataMem, NULL);
    vk::vkDestroyBuffer(*devicePointer, drawCmdbuf, NULL); 
    vk::vkFreeMemory(*devicePointer, drawCmdMem, NULL); 
}
void DrawableObjectCommonLight::drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer)
{
    vk::vkCmdBindPipeline(cmd,VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline);
    vk::vkCmdBindDescriptorSets(cmd,VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,desSetPointer, 0, NULL);
    const VkDeviceSize offsetsVertex[1] = {0};
    vk::vkCmdBindVertexBuffers(
            cmd,
            0,
            1,
            &(vertexDatabuf),
            offsetsVertex
    );
    float* mvp = MatrixState3D::getFinalMatrix();					
    memcpy(pushConstantData, mvp, sizeof(float) * 16);				
    vk::vkCmdPushConstants(cmd, pipelineLayout, 				
                           VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 16, pushConstantData);
    vk::vkCmdDrawIndirect(
            cmd, //当前使用的命令缓冲
            drawCmdbuf, //间接绘制信息数据缓冲
            0, //绘制信息数据的起始偏移量
            indirectDrawCount, //此次绘制使用的间接绘制信息组的数量
            sizeof(VkDrawIndirectCommand)); //每组绘制信息数据所占字节数
}
