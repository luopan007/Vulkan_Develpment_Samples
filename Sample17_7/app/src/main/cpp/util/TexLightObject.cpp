#include "TexLightObject.h"
#include "mylog.h"
#include <assert.h>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include <string.h>
#include "VertData.h"


void TexLightObject::createVertexDataBuffer(int dataByteCount,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
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
    uint8_t *cData=(uint8_t *)vdata;
    
    result = vk::vkMapMemory(device, vertexDataMem, 0, mem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    
    memcpy(pData, cData, dataByteCount);
    
    vk::vkUnmapMemory(device, vertexDataMem);

    
    result = vk::vkBindBufferMemory(device, vertexDatabuf, vertexDataMem, 0);
    assert(result == VK_SUCCESS);

    
    vertexDataBufferInfo.buffer = vertexDatabuf;
    vertexDataBufferInfo.offset = 0;
    vertexDataBufferInfo.range = mem_reqs.size;
}


void TexLightObject::createVertexDataBufferCompute(int dataByteCount, VkDevice &device,
                                                   VkPhysicalDeviceMemoryProperties &memoryroperties)
{
    
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    buf_info.size = dataByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(device, &buf_info, NULL, &vertexDatabufCompute);
    assert(result == VK_SUCCESS);

    
    VkMemoryRequirements mem_reqs;
    vk::vkGetBufferMemoryRequirements(device, vertexDatabufCompute, &mem_reqs);
    assert(dataByteCount<=mem_reqs.size);
    
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = mem_reqs.size;

    
    VkFlags requirements_mask=VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    
    bool flag=memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits,requirements_mask,&alloc_info.memoryTypeIndex);
    if(flag)
    {
        LOGE("确定内存类型成功 类型索引为%d",alloc_info.memoryTypeIndex);
    }
    else
    {
        LOGE("确定内存类型失败!");
    }

    
    result = vk::vkAllocateMemory(device, &alloc_info, NULL, &vertexDataMemCompute);
    assert(result == VK_SUCCESS);

    
    result = vk::vkBindBufferMemory(device, vertexDatabufCompute, vertexDataMemCompute, 0);
    assert(result == VK_SUCCESS);

    
    vertexDataBufferInfoCompute.buffer = vertexDatabufCompute;
    vertexDataBufferInfoCompute.offset = 0;
    vertexDataBufferInfoCompute.range = mem_reqs.size;
}


void TexLightObject::createVertexIndexDataBuffer(int indexDataByteCount,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    buf_info.size = indexDataByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(device, &buf_info, NULL, &vertexIndexDatabuf);
    assert(result == VK_SUCCESS);

    
    VkMemoryRequirements mem_reqs;
    vk::vkGetBufferMemoryRequirements(device, vertexIndexDatabuf, &mem_reqs);
    assert(indexDataByteCount<=mem_reqs.size);
    
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

    
    result = vk::vkAllocateMemory(device, &alloc_info, NULL, &vertexIndexDataMem);
    assert(result == VK_SUCCESS);


    LOGE("w-w %d %d",(int)(mem_reqs.size),indexDataByteCount);

    uint8_t *pData;
    
    result = vk::vkMapMemory(device, vertexIndexDataMem, 0, mem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    
    memcpy(pData, indexData, indexDataByteCount);
    
    vk::vkUnmapMemory(device, vertexIndexDataMem);

    
    result = vk::vkBindBufferMemory(device, vertexIndexDatabuf, vertexIndexDataMem, 0);
    assert(result == VK_SUCCESS);
}


TexLightObject::TexLightObject(float* vdataIn,int vdataByteCount,int vCountIn,int* indexDataIn,int indexDataByteCount,int indexCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    pushConstantData=new float[32];

    this->devicePointer=&device;
    this->vdata=vdataIn;
    this->vCount=vCountIn;

    this->indexData=indexDataIn;
    this->indexCount=indexCountIn;

    createVertexDataBuffer(vdataByteCount,device,memoryroperties);
    createVertexDataBufferCompute(vdataByteCount, device, memoryroperties);
    createVertexIndexDataBuffer(indexDataByteCount,device,memoryroperties);
}

TexLightObject::~TexLightObject()
{
    delete[] vdata;
    delete[] pushConstantData;
    delete[] indexData;
    vk::vkDestroyBuffer(*devicePointer, vertexDatabuf, NULL);
    vk::vkFreeMemory(*devicePointer, vertexDataMem, NULL);
    vk::vkDestroyBuffer(*devicePointer, vertexDatabufCompute, NULL);
    vk::vkFreeMemory(*devicePointer, vertexDataMemCompute, NULL);
    vk::vkDestroyBuffer(*devicePointer, vertexIndexDatabuf, NULL);
    vk::vkFreeMemory(*devicePointer, vertexIndexDataMem, NULL);
}


void TexLightObject::drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer)
{
    
    vk::vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline);
    
    vk::vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,desSetPointer, 0, NULL);
    
    const VkDeviceSize offsetsVertex[1] = {0};
    vk::vkCmdBindVertexBuffers
    (
            cmd,
            0,
            1,
            &(vertexDatabufCompute),
            offsetsVertex
    );
    
    vk::vkCmdBindIndexBuffer(cmd,vertexIndexDatabuf,0,VK_INDEX_TYPE_UINT32);

    float* mvp=MatrixState3D::getFinalMatrix();
    float* mm=MatrixState3D::getMMatrix();
    memcpy(pushConstantData, mvp, sizeof(float)*16);
    memcpy(pushConstantData+16, mm, sizeof(float)*16);
    vk::vkCmdPushConstants(cmd, pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float)*32,pushConstantData);
    
    vk::vkCmdDrawIndexed(cmd,indexCount,1,0,0,0);
}


void TexLightObject::calSelfBD(VkCommandBuffer& cmd,
                               VkPipelineLayout &pipelineLayout, VkPipeline &pipeline,
                               VkDescriptorSet *desSetPointer)
{
    
    vk::vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE,pipeline);
    
    vk::vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1,desSetPointer, 0, NULL);
    
    const VkDeviceSize offsetsVertex[2] = {0,1};
    VkBuffer pBuffers[2]={vertexDatabuf,vertexDatabufCompute};
    vk::vkCmdBindVertexBuffers
    (
            cmd,
            0,
            2,
            pBuffers,
            offsetsVertex
    );

    
    uint32_t size=CR+1;
    vk::vkCmdDispatch(cmd,size,size,1);
}


void TexLightObject::calSelfNormal(VkCommandBuffer& cmd,
                                   VkPipelineLayout &pipelineLayout, VkPipeline &pipeline,
                                   VkDescriptorSet *desSetPointer)
{
    
    vk::vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE,pipeline);
    
    vk::vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1,desSetPointer, 0, NULL);
    
    const VkDeviceSize offsetsVertex[2] = {0};
    VkBuffer pBuffers[2]={vertexDatabufCompute};
    vk::vkCmdBindVertexBuffers
    (
            cmd,
            0,
            1,
            pBuffers,
            offsetsVertex
    );
    
    uint32_t size=CR+1;
    vk::vkCmdDispatch(cmd,size,size,1);
}