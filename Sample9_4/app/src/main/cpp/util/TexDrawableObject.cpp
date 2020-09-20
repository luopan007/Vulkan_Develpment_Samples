#include "TexDrawableObject.h"
#include "mylog.h"
#include <assert.h>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include <string.h>
TexDrawableObject::TexDrawableObject(float* vdataIn,int dataByteCount,
                                     int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties) {
    pushConstantData=new float[36]; //创建推送常量数据数组
    this->devicePointer=&device;
    this->vdata=vdataIn;
    this->vCount=vCountIn;
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
    assert(flag);
    LOGE("确定内存类型成功 类型索引为%d",alloc_info.memoryTypeIndex);
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
TexDrawableObject::~TexDrawableObject()
{
    delete [] vdata;
    delete [] pushConstantData;
    vk::vkDestroyBuffer(*devicePointer, vertexDatabuf, NULL);
    vk::vkFreeMemory(*devicePointer, vertexDataMem, NULL);
}
void TexDrawableObject::drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout, VkPipeline& pipeline,VkDescriptorSet* desSetPointer) { //绘制方法
    vk::vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline);
    vk::vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,desSetPointer, 0, NULL);
    const VkDeviceSize offsetsVertex[1] = {0};
    vk::vkCmdBindVertexBuffers
    (
            cmd,
            0,
            1,
            &(vertexDatabuf),
            offsetsVertex
    );
    float* mvp=MatrixState3D::getFinalMatrix(); //获取最终变换矩阵
    float* mm = MatrixState3D::getMMatrix(); //获取基本变换矩阵
    float camea[4] = { MatrixState3D::cx,MatrixState3D::cy,MatrixState3D::cz,1.0};//获取摄像机参数
    memcpy(pushConstantData, mvp, sizeof(float)*16); //将最终变换矩阵数据拷贝进推送常量
    memcpy(pushConstantData + 16, mm, sizeof(float) * 16); //将基本变换矩阵数据拷贝进推送常量
    memcpy(pushConstantData + 32, camea, sizeof(float) * 4);//将摄像机参数数据拷贝进推送常量
    vk::vkCmdPushConstants(cmd, pipelineLayout, //将推送常量数据送入管线
    VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float)*36,pushConstantData);
    vk::vkCmdDraw(cmd, vCount, 1, 0, 0); //执行绘制
}