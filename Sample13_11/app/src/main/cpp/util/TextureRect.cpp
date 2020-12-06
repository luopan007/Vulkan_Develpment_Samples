#include "TextureRect.h"
#include <assert.h>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include "Constant.h"
#include "CalNormal.h"
#include <string.h>
#include <mylog.h>
TextureRect::TextureRect(float* vdataIn, int vdataByteCount, int vCountIn, int* indexDataIn, int indexDataByteCount, int indexCountIn, VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    pushConstantData = new float[32];
    this->devicePointer = &device;
    this->vdata = vdataIn;
    this->vCount = vCountIn;
    this->vdataByteCount = vdataByteCount;
    this->indexData = indexDataIn;
    this->indexCount = indexCountIn;
    vdataForCal = new float[vCountIn * 3];
    ndataForCal = new float[vCountIn * 3];
    vdataForTrans = new float[vCountIn * 8];
    memcpy(vdataForTrans, vdata, vdataByteCount);
    zero1 = new float[3] {wave1PositionX, wave1PositionY, wave1PositionZ};
    zero2 = new float[3] {wave2PositionX, wave2PositionY, wave2PositionZ};
    zero3 = new float[3] {wave3PositionX, wave3PositionY, wave3PositionZ};
    createVertexDataBuffer(vdataByteCount, device, memoryroperties);
    createVertexIndexDataBuffer(indexDataByteCount, device, memoryroperties);
}
void TextureRect::createVertexDataBuffer(int vdataByteCount, VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.size = vdataByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(device, &buf_info, NULL, &vertexDatabuf);
    assert(result == VK_SUCCESS);
    vk::vkGetBufferMemoryRequirements(device, vertexDatabuf, &vdataMem_reqs);
    assert(vdataByteCount <= vdataMem_reqs.size);
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = vdataMem_reqs.size;
    VkFlags requirements_mask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bool flag = memoryTypeFromProperties(memoryroperties, vdataMem_reqs.memoryTypeBits, requirements_mask, &alloc_info.memoryTypeIndex);
    assert(flag);
    LOGE("确定内存类型成功 类型索引为%d", alloc_info.memoryTypeIndex);
    result = vk::vkAllocateMemory(device, &alloc_info, NULL, &vertexDataMem);
    assert(result == VK_SUCCESS);
    uint8_t *pData;
    result = vk::vkMapMemory(device, vertexDataMem, 0, vdataMem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, vdata, vdataByteCount);
    vk::vkUnmapMemory(device, vertexDataMem);
    result = vk::vkBindBufferMemory(device, vertexDatabuf, vertexDataMem, 0);
    assert(result == VK_SUCCESS);
    vertexDataBufferInfo.buffer = vertexDatabuf;
    vertexDataBufferInfo.offset = 0;
    vertexDataBufferInfo.range = vdataMem_reqs.size;
}
void TextureRect::createVertexIndexDataBuffer(int indexDataByteCount, VkDevice& device, VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.size = indexDataByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(device, &buf_info, NULL, &vertexIndexDatabuf);
    assert(result == VK_SUCCESS);
    VkMemoryRequirements mem_reqs;
    vk::vkGetBufferMemoryRequirements(device, vertexIndexDatabuf, &mem_reqs);
    assert(indexDataByteCount <= mem_reqs.size);
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = mem_reqs.size;
    VkFlags requirements_mask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bool flag = memoryTypeFromProperties(memoryroperties, mem_reqs.memoryTypeBits, requirements_mask, &alloc_info.memoryTypeIndex);
    assert(flag);
    LOGE("确定内存类型成功 类型索引为%d\n", alloc_info.memoryTypeIndex);
    result = vk::vkAllocateMemory(device, &alloc_info, NULL, &vertexIndexDataMem);
    assert(result == VK_SUCCESS);
    uint8_t *pData;
    result = vk::vkMapMemory(device, vertexIndexDataMem, 0, mem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, indexData, indexDataByteCount);
    vk::vkUnmapMemory(device, vertexIndexDataMem);
    result = vk::vkBindBufferMemory(device, vertexIndexDatabuf, vertexIndexDataMem, 0);
    assert(result == VK_SUCCESS);
    vertexIndexDataBufferInfo.buffer = vertexIndexDatabuf;
    vertexIndexDataBufferInfo.offset = 0;
    vertexIndexDataBufferInfo.range = mem_reqs.size;
}
void TextureRect::calVerticesNormalAndTangent(){ //计算顶点坐标与法向量的方法
    for (int i = 0; i<vCount; i++){ //遍历所有顶点
        vdataForCal[i * 3 + 0] = vdataForTrans[i * 8 + 0]; //复制顶点坐标x 分量
        vdataForCal[i * 3 + 1] = //计算顶点坐标y 分量
                findHeight(vdataForTrans[i * 8 + 0], vdataForTrans[i * 8 + 2]);
        vdataForCal[i * 3 + 2] = vdataForTrans[i * 8 + 2]; //复制顶点坐标z 分量
    }
    CalNormal::calNormal(vdataForTrans,vCount,indexData,indexCount,ndataForCal);//计算顶点法向量
}
float TextureRect::findHeight(float x, float z){ //计算3 个波影响后顶点高度值
    float result = 0; //存放结果的辅助变量
    float distance1 = (float)sqrt( //计算顶点距离1 号波振源位置的距离
            (x - zero1[0])*(x - zero1[0]) + (z - zero1[2])*(z - zero1[2]));
    float distance2 = (float)sqrt( //计算顶点距离2 号波振源位置的距离
            (x - zero2[0])*(x - zero2[0]) + (z - zero2[2])*(z - zero2[2]));
    float distance3 = (float)sqrt( //计算顶点距离3 号波振源位置的距离
            (x - zero3[0])*(x - zero3[0]) + (z - zero3[2])*(z - zero3[2]));
    result = (float)(sin((distance1)* //计算1 号波影响后的顶点高度值
                         waveFrequency1 * PI + mytime) *waveAmplitude1);
    result = (float)(result + sin((distance2)* //计算2 号波影响后的顶点高度值
                                  waveFrequency2 * PI + mytime)*waveAmplitude2);
    result = (float)(result + sin((distance3)* //计算3 号波影响后的顶点高度值
                                  waveFrequency3 * PI + mytime)*waveAmplitude3);
    return result; //返回最终结果
}
void TextureRect::copyData()
{
    for (int i = 0; i < vCount; i++)
    {
        vdataForTrans[i * 8 + 0] = vdataForCal[i * 3 + 0];
        vdataForTrans[i * 8 + 1] = vdataForCal[i * 3 + 1];
        vdataForTrans[i * 8 + 2] = vdataForCal[i * 3 + 2];
        vdataForTrans[i * 8 + 3] = vdataForTrans[i * 8 + 3];
        vdataForTrans[i * 8 + 4] = vdataForTrans[i * 8 + 4];
        vdataForTrans[i * 8 + 5] = ndataForCal[i * 3 + 0];
        vdataForTrans[i * 8 + 6] = ndataForCal[i * 3 + 1];
        vdataForTrans[i * 8 + 7] = ndataForCal[i * 3 + 2];
    }
}
void TextureRect::referVertexBuffer()
{
    memcpy(vdata, vdataForTrans, vdataByteCount);
}
void TextureRect::drawSelf(VkCommandBuffer& cmd, VkPipelineLayout& pipelineLayout, VkPipeline& pipeline, VkDescriptorSet* desSetPointer)
{
    uint8_t *pData;
    VkResult result = vk::vkMapMemory(*devicePointer, vertexDataMem, 0, vdataMem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, vdata, vdataByteCount);
    vk::vkUnmapMemory(*devicePointer, vertexDataMem);
    vk::vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    vk::vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, desSetPointer, 0, NULL);
    const VkDeviceSize offsetsVertex[1] = { 0 };
    vk::vkCmdBindVertexBuffers
            (
                    cmd,
                    0,
                    1,
                    &(vertexDatabuf),
                    offsetsVertex
            );
    vk:: vkCmdBindIndexBuffer(cmd, vertexIndexDatabuf, 0, VK_INDEX_TYPE_UINT32);
    float* mvp = MatrixState3D::getFinalMatrix();
    float* mm = MatrixState3D::getMMatrix();
    memcpy(pushConstantData, mvp, sizeof(float) * 16);
    memcpy(pushConstantData + 16, mm, sizeof(float) * 16);
    vk::vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 32, pushConstantData);
    vk::vkCmdDrawIndexed(cmd, indexCount, 1, 0, 0, 0);
}
TextureRect::~TextureRect()
{
    delete[] vdata;
    vk::vkDestroyBuffer(*devicePointer, vertexDatabuf, NULL);
    vk::vkFreeMemory(*devicePointer, vertexDataMem, NULL);
    delete[] indexData;
    vk::vkDestroyBuffer(*devicePointer, vertexIndexDatabuf, NULL);
    vk::vkFreeMemory(*devicePointer, vertexIndexDataMem, NULL);
}
