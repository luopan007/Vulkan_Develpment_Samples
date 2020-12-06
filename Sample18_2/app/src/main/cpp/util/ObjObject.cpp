#include "ObjObject.h"
#include "mylog.h"
#include <assert.h>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include "AABB.h"
ObjObject::ObjObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    pushConstantData=new float[36];
    this->devicePointer=&device;
    this->vdata=vdataIn;
    this->vCount=vCountIn;
    createAABB();
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
    result = vk::vkAllocateMemory(device,&alloc_info, NULL, &vertexDataMem);
    assert(result == VK_SUCCESS);
    uint8_t *pData;
    result = vk::vkMapMemory(device,vertexDataMem, 0, mem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, vdata, dataByteCount);
    vk::vkUnmapMemory(device, vertexDataMem);
    result = vk::vkBindBufferMemory(device,vertexDatabuf, vertexDataMem, 0);
    assert(result == VK_SUCCESS);
    vertexDataBufferInfo.buffer = vertexDatabuf;
    vertexDataBufferInfo.offset = 0;
    vertexDataBufferInfo.range = mem_reqs.size;
}
ObjObject::~ObjObject()
{
    delete [] vdata;
    delete [] pushConstantData;
    delete preBox;
    vk::vkDestroyBuffer(*devicePointer, vertexDatabuf, NULL);
    vk::vkFreeMemory(*devicePointer, vertexDataMem, NULL);
}
void ObjObject::drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer)
{
    copyM();
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
    float* mvp = MatrixState3D::getFinalMatrix();
    float* mm = MatrixState3D::getMMatrix();
    memcpy(pushConstantData, mvp, sizeof(float) * 16);
    memcpy(pushConstantData + 16, mm, sizeof(float) * 16);
    memcpy(pushConstantData + 32, color, sizeof(float) * 4);
    vk::vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 32, pushConstantData);
    vk::vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(float) * 32, sizeof(float) * 4, pushConstantData +32);
    vk::vkCmdDraw(cmd, vCount, 1, 0, 0);
}
void ObjObject::createAABB(){ //创建物体AABB 包围盒的方法
    float minX = FLT_MAX;float maxX = -FLT_MAX; //初始化AABB 包围盒的6 个参数
    float minY = FLT_MAX;float maxY = -FLT_MAX;
    float minZ = FLT_MAX;float maxZ = -FLT_MAX;
    for (int i = 0; i < vCount; i++){ //遍历物体的所有顶点
        float currX=vdata[i*6+0];float currY=vdata[i*6+1];float currZ=vdata[i*6+2];//获取当前顶点的坐标
        if (minX > currX) { //若原minX 值大于当前顶点X 坐标
            minX = currX; //更新AABB 包围盒的minX 值
        }
        if (maxX < currX) { //若原maxX 值小于当前顶点X 坐标
            maxX = currX; //更新AABB 包围盒的maxX 值
        }
        if (minY > currY) { minY = currY; }
        if (maxY < currY) { maxY = currY; }
        if (minZ > currZ) { minZ = currZ; }
        if (maxZ < currZ) { maxZ = currZ; }
    }
    preBox = new AABB(minX, maxX, minY, maxY, minZ, maxZ);//根据6 个参数创建AABB 包围盒对象
}