#include "TexLeafDrawableObject.h"
#include "mylog.h"
#include <assert.h>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include "Constant.h"
#include <string.h>
#include <TreeControl.h>

/*
 * pxIn pyIn pzIn 为树的底部中心点坐标
 * tangle 为树叶的旋转角度
 * */
TexLeafDrawableObject::TexLeafDrawableObject(float* vdataIn,int dataByteCount,int vCountIn,VkDevice& device,
                                             VkPhysicalDeviceMemoryProperties& memoryroperties,float pxIn,float pyIn,float pzIn,float tangle,float idIn)
{
    pushConstantData=new float[16];
    this->devicePointer=&device;
    this->vdata=vdataIn;
    this->vCount=vCountIn;
    this->postionX=pxIn;
    this->postionY=pyIn;
    this->postionZ=pzIn;
    this->centerx=pxIn+Constant::leaves_width/2*cos(toRadians(tangle));//计算当前中心X坐标
    this->centery=pyIn+Constant::LEAVE_CENTER_HEIGHT;//计算当前中心Y坐标
    this->centerz=pzIn+Constant::leaves_height/2*sin(toRadians(tangle));//计算当前中心Z坐标
    float *initialPos=new float[3]{
            centerx, centery,centerz
    };
    TreeControl::leafInitialPos.push_back(initialPos);
    this->centerAngle=tangle;
    this->id=idIn;
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
TexLeafDrawableObject::~TexLeafDrawableObject()
{
    delete vdata;
    vk::vkDestroyBuffer(*devicePointer, vertexDatabuf, NULL);
    vk::vkFreeMemory(*devicePointer, vertexDataMem, NULL);
}
void TexLeafDrawableObject::drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer)
{
    MatrixState3D::pushMatrix();
    MatrixState3D::rotate(centerAngle,0,1,0);
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
    float* mvp=MatrixState3D::getFinalMatrix();
    memcpy(pushConstantData, mvp, sizeof(float)*16);
    vk::vkCmdPushConstants(cmd, pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float)*16,pushConstantData);
    vk::vkCmdDraw(cmd, vCount, 1, 0, 0);
    MatrixState3D::popMatrix();
}
