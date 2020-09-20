#include "DrawableObjectCommon.h"
#include "mylog.h"
#include <assert.h>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include <string.h>
DrawableObjectCommonLight::DrawableObjectCommonLight( 
        float* vdataIn,int dataByteCount,int vCountIn, 
        uint16_t* idataIn,int indexByteCount,int iCountIn, 
        VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    pushConstantData=new float[16]; 
    this->devicePointer=&device; 
    this->vdata=vdataIn; 
    this->vCount=vCountIn; 
    this->idata=idataIn; 
    this->iCount=iCountIn; 
    createVertexBuffer(dataByteCount,device,memoryroperties); 
    createIndexBuffer(indexByteCount,device,memoryroperties); 
    
    initDrawCmdbuf(device, memoryroperties);
}
void DrawableObjectCommonLight::createVertexBuffer(int dataByteCount,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.size = dataByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(device, &buf_info, NULL, &vertexDatabuf);
    assert(result == VK_SUCCESS);
    
    VkMemoryRequirements mem_reqs;
    vk::vkGetBufferMemoryRequirements(device, vertexDatabuf, &mem_reqs);
    assert(dataByteCount<=mem_reqs.size);
    
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = mem_reqs.size;
    
    VkFlags requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    
    bool flag=memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits,requirements_mask,&alloc_info.memoryTypeIndex);
    if(flag)
    {
        LOGE("确定内存类型成功 类型索引为%d",alloc_info.memoryTypeIndex);
    }
    else
    {
        LOGE("确定内存类型失败!");
    }
    
    result = vk::vkAllocateMemory(device, &alloc_info, NULL, &vertexDataMem);
    assert(result == VK_SUCCESS);
    uint8_t *pData;
    
    result = vk::vkMapMemory(device, vertexDataMem, 0, mem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    
    memcpy(pData, vdata, dataByteCount);
    
    vk::vkUnmapMemory(device, vertexDataMem);
    
    result = vk::vkBindBufferMemory(device, vertexDatabuf, vertexDataMem, 0);
    assert(result == VK_SUCCESS);
    
    vertexDataBufferInfo.buffer = vertexDatabuf;
    vertexDataBufferInfo.offset = 0;
    vertexDataBufferInfo.range = mem_reqs.size;
}
void DrawableObjectCommonLight::createIndexBuffer(int indexByteCount,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    
    VkBufferCreateInfo index_buf_info = {};
    index_buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    index_buf_info.pNext = NULL;
    index_buf_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    index_buf_info.size = indexByteCount;
    index_buf_info.queueFamilyIndexCount = 0;
    index_buf_info.pQueueFamilyIndices = NULL;
    index_buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    index_buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(device, &index_buf_info, NULL, &indexDatabuf);
    assert(result == VK_SUCCESS);
    
    VkMemoryRequirements index_mem_reqs;
    vk::vkGetBufferMemoryRequirements(device, indexDatabuf, &index_mem_reqs);
    assert(indexByteCount<=index_mem_reqs.size);
    
    VkMemoryAllocateInfo index_alloc_info = {};
    index_alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    index_alloc_info.pNext = NULL;
    index_alloc_info.memoryTypeIndex = 0;
    index_alloc_info.allocationSize = index_mem_reqs.size;
    
    VkFlags index_requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    
    bool index_flag=memoryTypeFromProperties(memoryroperties, index_mem_reqs.memoryTypeBits,index_requirements_mask,&index_alloc_info.memoryTypeIndex);
    if(index_flag)
    {
        LOGE("确定内存类型成功 类型索引为%d",index_alloc_info.memoryTypeIndex);
    }
    else
    {
        LOGE("确定内存类型失败!");
    }
    
    result = vk::vkAllocateMemory(device, &index_alloc_info, NULL, &indexDataMem);
    assert(result == VK_SUCCESS);
    uint8_t *index_pData;
    
    result = vk::vkMapMemory(device, indexDataMem, 0, index_mem_reqs.size, 0, (void **)&index_pData);
    assert(result == VK_SUCCESS);
    
    memcpy(index_pData, idata, indexByteCount);
    
    vk::vkUnmapMemory(device, indexDataMem);
    
    result = vk::vkBindBufferMemory(device, indexDatabuf, indexDataMem, 0);
    assert(result == VK_SUCCESS);
    
    indexDataBufferInfo.buffer = indexDatabuf;
    indexDataBufferInfo.offset = 0;
    indexDataBufferInfo.range = index_mem_reqs.size;
}
void DrawableObjectCommonLight::initDrawCmdbuf( //用于创建间接绘制信息数据缓冲的方法
        VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryroperties){
    indirectDrawCount = 2; //间接绘制信息数据组的数量
    drawCmdbufbytes=indirectDrawCount*sizeof(VkDrawIndexedIndirectCommand);
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    buf_info.size = drawCmdbufbytes;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(device, &buf_info, NULL, &drawCmdbuf);
    assert(result == VK_SUCCESS);
    VkMemoryRequirements mem_reqs;
    vk::vkGetBufferMemoryRequirements(device, drawCmdbuf, &mem_reqs);
    assert(drawCmdbufbytes <= mem_reqs.size);
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = mem_reqs.size;
    VkFlags requirements_mask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bool flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits, requirements_mask, &alloc_info.memoryTypeIndex);
    if (flag)
    {
        LOGE("确定内存类型成功 类型索引为%d", alloc_info.memoryTypeIndex);
    }
    else
    {
        LOGE("确定内存类型失败!");
    }
    result = vk::vkAllocateMemory(device, &alloc_info, NULL, &drawCmdMem);
    assert(result == VK_SUCCESS);
    uint8_t *pData;
    result = vk::vkMapMemory(device, drawCmdMem, 0, mem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);

    VkDrawIndexedIndirectCommand dic[2]; //创建间接绘制信息结构体实例数组
    dic[0].indexCount= iCount; //第1 组绘制信息数据的索引数量
    dic[0].instanceCount=1; //第1 组绘制信息数据的实例数量
    dic[0].firstIndex=0; //第1 组绘制信息数据的绘制用起始索引
    dic[0].vertexOffset=0; //第1 组绘制信息数据的顶点数据偏移量
    dic[0].firstInstance=0; //第1 组绘制信息数据的首实例索引
    dic[1].indexCount = iCount/2+1; //第2 组绘制信息数据的索引数量
    dic[1].instanceCount = 1; //第2 组绘制信息数据的实例数量
    dic[1].firstIndex = 0; //第2 组绘制信息数据的绘制用起始索引
    dic[1].vertexOffset = 0; //第2 组绘制信息数据的顶点数据偏移量
    dic[1].firstInstance = 0; //第2 组绘制信息数据的首实例索引
    memcpy(pData, &dic, drawCmdbufbytes); //将数据拷贝进设备内存
    vk::vkUnmapMemory(device, vertexDataMem); //解除内存映射
    result = vk::vkBindBufferMemory(device, drawCmdbuf, drawCmdMem, 0); //绑定内存与缓冲
    assert(result == VK_SUCCESS); //检查绑定是否成功
}

DrawableObjectCommonLight::~DrawableObjectCommonLight()
{
    delete[] vdata; 
    vk::vkDestroyBuffer(*devicePointer, vertexDatabuf, NULL); 
    vk::vkFreeMemory(*devicePointer, vertexDataMem, NULL); 
    delete[] idata; 
    vk::vkDestroyBuffer(*devicePointer, indexDatabuf, NULL); 
    vk::vkFreeMemory(*devicePointer, indexDataMem, NULL); 
    vk::vkDestroyBuffer(*devicePointer, drawCmdbuf, NULL); 
    vk::vkFreeMemory(*devicePointer, drawCmdMem, NULL); 
}
void DrawableObjectCommonLight::drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer,int cmdDataOffset)
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
    vk::vkCmdBindIndexBuffer ( 
            cmd, 
            indexDatabuf, 
            0, 
            VK_INDEX_TYPE_UINT16);
    vk::vkCmdDrawIndexedIndirect(
            cmd, //当前使用的命令缓冲
            drawCmdbuf, //间接绘制信息数据缓冲
            cmdDataOffset, //绘制信息数据的起始偏移量（以字节计）
            1, //此次绘制使用的间接绘制信息组的数量
            sizeof(VkDrawIndexedIndirectCommand)); //每组绘制信息数据所占字节数
}
