#include "DrawableObjectCommon.h"
#include "mylog.h"
#include <assert.h>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include <string.h>

DrawableObjectCommonLight::DrawableObjectCommonLight( //构造函数
        float* vdataIn,int dataByteCount,int vCountIn, //传入的顶点数据相关参数
        uint16_t* idataIn,int indexByteCount,int iCountIn, //传入的索引数据相关参数
        VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    pushConstantData=new float[16]; //创建推送常量数据数组
    this->devicePointer=&device; //接收逻辑设备指针并保存
    this->vdata=vdataIn; //接收顶点数据数组首地址指针并保存
    this->vCount=vCountIn; //接收顶点数量并保存
    this->idata=idataIn; //接收索引数据数组首地址指针并保存
    this->iCount=iCountIn; //接收索引数量并保存
    createVertexBuffer(dataByteCount,device,memoryroperties); //调用方法创建顶点数据缓冲
    createIndexBuffer(indexByteCount,device,memoryroperties); //调用方法创建索引数据缓冲
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

    //需要的内存类型掩码
    VkFlags requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    //获取所需内存类型索引
    bool flag=memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits,requirements_mask,&alloc_info.memoryTypeIndex);
    if(flag)
    {
        LOGE("确定内存类型成功 类型索引为%d",alloc_info.memoryTypeIndex);
    }
    else
    {
        LOGE("确定内存类型失败!");
    }
    //为顶点数据缓冲分配内存
    result = vk::vkAllocateMemory(device, &alloc_info, NULL, &vertexDataMem);
    assert(result == VK_SUCCESS);

    uint8_t *pData;
    //将显存映射为CPU可访问
    result = vk::vkMapMemory(device, vertexDataMem, 0, mem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);

    //将顶点数据拷贝进显存
    memcpy(pData, vdata, dataByteCount);
    //解除内存映射
    vk::vkUnmapMemory(device, vertexDataMem);

    //绑定内存与缓冲
    result = vk::vkBindBufferMemory(device, vertexDatabuf, vertexDataMem, 0);
    assert(result == VK_SUCCESS);

    //记录Buffer Info
    vertexDataBufferInfo.buffer = vertexDatabuf;
    vertexDataBufferInfo.offset = 0;
    vertexDataBufferInfo.range = mem_reqs.size;
}

void DrawableObjectCommonLight::createIndexBuffer(int indexByteCount,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    //创建Buffer创建信息实例，为创建索引数据Buffer服务
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

    //获取内存需求
    VkMemoryRequirements index_mem_reqs;
    vk::vkGetBufferMemoryRequirements(device, indexDatabuf, &index_mem_reqs);
    assert(indexByteCount<=index_mem_reqs.size);
    //内存分配信息
    VkMemoryAllocateInfo index_alloc_info = {};
    index_alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    index_alloc_info.pNext = NULL;
    index_alloc_info.memoryTypeIndex = 0;
    index_alloc_info.allocationSize = index_mem_reqs.size;

    //需要的内存类型掩码
    VkFlags index_requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    //获取所需内存类型索引
    bool index_flag=memoryTypeFromProperties(memoryroperties, index_mem_reqs.memoryTypeBits,index_requirements_mask,&index_alloc_info.memoryTypeIndex);
    if(index_flag)
    {
        LOGE("确定内存类型成功 类型索引为%d",index_alloc_info.memoryTypeIndex);
    }
    else
    {
        LOGE("确定内存类型失败!");
    }
    //为顶点数据缓冲分配内存
    result = vk::vkAllocateMemory(device, &index_alloc_info, NULL, &indexDataMem);
    assert(result == VK_SUCCESS);

    uint8_t *index_pData;
    //将显存映射为CPU可访问
    result = vk::vkMapMemory(device, indexDataMem, 0, index_mem_reqs.size, 0, (void **)&index_pData);
    assert(result == VK_SUCCESS);

    //将顶点数据拷贝进显存
    memcpy(index_pData, idata, indexByteCount);
    //解除内存映射
    vk::vkUnmapMemory(device, indexDataMem);

    //绑定内存与缓冲
    result = vk::vkBindBufferMemory(device, indexDatabuf, indexDataMem, 0);
    assert(result == VK_SUCCESS);

    //记录Buffer Info
    indexDataBufferInfo.buffer = indexDatabuf;
    indexDataBufferInfo.offset = 0;
    indexDataBufferInfo.range = index_mem_reqs.size;
}

DrawableObjectCommonLight::~DrawableObjectCommonLight()
{
    delete[] vdata; //释放顶点数据内存
    vk::vkDestroyBuffer(*devicePointer, vertexDatabuf, NULL); //销毁顶点数据缓冲
    vk::vkFreeMemory(*devicePointer, vertexDataMem, NULL); //释放顶点数据缓冲对应设备内存
    delete[] idata; //释放索引数据内存
    vk::vkDestroyBuffer(*devicePointer, indexDatabuf, NULL); //销毁索引数据缓冲
    vk::vkFreeMemory(*devicePointer, indexDataMem, NULL); //释放索引数据缓冲对应设备内存
}

void DrawableObjectCommonLight::drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer,uint32_t sIndex,uint32_t eIndex)
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
    float* mvp = MatrixState3D::getFinalMatrix();					//获取总变换矩阵
    memcpy(pushConstantData, mvp, sizeof(float) * 16);				//将总变换矩阵拷贝入内存
    vk::vkCmdPushConstants(cmd, pipelineLayout, 				//将常量数据送入管线
                           VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 16, pushConstantData);
    vk::vkCmdBindIndexBuffer ( //将顶点数据与当前使用的命令缓冲绑定
            cmd, //当前使用的命令缓冲
            indexDatabuf, //索引数据缓冲
            0, //索引数据缓冲首索引
            VK_INDEX_TYPE_UINT16); //索引数据
    vk::vkCmdDrawIndexed( //执行索引绘制
            cmd, //当前使用的命令缓冲
            eIndex-sIndex, //索引数量
            1, //需要绘制的实例数量
            sIndex, //绘制用起始索引
            0, //顶点数据偏移量
            0); //需要绘制的第1 个实例的索引
}