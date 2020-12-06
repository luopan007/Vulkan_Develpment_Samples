#include <MyVulkanManager.h>
#include <../util/TextureManager.h>
#include "BnggdhDraw.h"
BnggdhDraw::BnggdhDraw(string sourceName,
                       VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties) {
    pushConstantData = new float[16]; //创建推送常量数据数组
    this->devicePointer = &device; //接收逻辑设备指针并保存
    this->memoryPropertiesPointer = &memoryroperties; //接收物理设备内存属性列表并保存
    bnggdh = new Bnggdh(sourceName); //创建Bnggdh 类对象
    initVertexData(); //初始化顶点数据
    initIndexData(); //初始化索引数据
    initVertexBuffer(); //创建顶点数据缓冲
    initIndexBuffer(); //创建索引数据缓冲
}
void BnggdhDraw::initVertexData() { //初始化顶点数据的方法
    maxKeyTime = bnggdh->getMaxKeytime(); //获取最大关键帧的时间
    vCount= bnggdh->getVerNums(); //获取顶点数量
    vdata=new float[vCount*5](); //初始化顶点数据数组
    vDataTransfer = new float[vCount * 5](); //初始化中间传输区顶点数据数组
    vdataByteCount=vCount*5* sizeof(float); //计算顶点数据所占总字节数
    int count=0; //辅助索引
    positionData=bnggdh->getPosition(); //获取顶点位置坐标数据
    textureData=bnggdh->getTextures(); //获取顶点纹理坐标数据
    for(int i=0;i<vCount;i++){ //遍历顶点数据数组的每个元素
        vdata[count++]=positionData[i*3+0]; //顶点位置坐标的X 分量
        vdata[count++]=positionData[i*3+1]; //顶点位置坐标的Y 分量
        vdata[count++]=positionData[i*3+2]; //顶点位置坐标的Z 分量
        vdata[count++]=textureData[i*2+0]; //顶点的纹理S 坐标
        vdata[count++]=textureData[i*2+1]; //顶点的纹理T 坐标
    }
	count = 0;
	for (int i = 0;i<vCount;i++)
	{
		vDataTransfer[count++] = positionData[i * 3 + 0];
		vDataTransfer[count++] = positionData[i * 3 + 1];
		vDataTransfer[count++] = positionData[i * 3 + 2];
		vDataTransfer[count++] = textureData[i * 2 + 0];
		vDataTransfer[count++] = textureData[i * 2 + 1];
	}
}
void BnggdhDraw::initIndexData() { //初始化索引数据的方法
    iCount= bnggdh->getIndexNums(); //获取索引数量
    idata=new uint16_t[iCount](); //初始化索引数据数组
    idataByteCount=iCount * sizeof(uint16_t); //计算索引数据所占总字节数
    for(int i=0;i<iCount;i++){ //遍历索引数据数组的每个元素
        idata[i]=bnggdh->getIndices()[i]; //获取顶点数据索引
    }}
void BnggdhDraw::initVertexBuffer(){
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.size = vdataByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(*devicePointer, &buf_info, NULL, &vertexDatabuf);
    assert(result == VK_SUCCESS);
    vk::vkGetBufferMemoryRequirements(*devicePointer, vertexDatabuf, &vmem_reqs);
    assert(vdataByteCount<=vmem_reqs.size);
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = vmem_reqs.size;
    VkFlags requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bool flag=memoryTypeFromProperties(*memoryPropertiesPointer, vmem_reqs.memoryTypeBits,requirements_mask,&alloc_info.memoryTypeIndex);
    if(flag)
    {
        LOGE("确定内存类型成功 类型索引为%d\n",alloc_info.memoryTypeIndex);
    }
    else
    {
        LOGE("确定内存类型失败!\n");
    }
    result = vk::vkAllocateMemory(*devicePointer,&alloc_info, NULL, &vertexDataMem);
    assert(result == VK_SUCCESS);
    uint8_t *pData;
    result = vk::vkMapMemory(*devicePointer,vertexDataMem, 0, vmem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, vdata, vdataByteCount);
    vk::vkUnmapMemory(*devicePointer, vertexDataMem);
    result = vk::vkBindBufferMemory(*devicePointer,vertexDatabuf, vertexDataMem, 0);
    assert(result == VK_SUCCESS);
    vertexDataBufferInfo.buffer = vertexDatabuf;
    vertexDataBufferInfo.offset = 0;
    vertexDataBufferInfo.range = vmem_reqs.size;
}
void BnggdhDraw::initIndexBuffer(){
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.size = idataByteCount;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult result = vk::vkCreateBuffer(*devicePointer, &buf_info, NULL, &indexDatabuf);
    assert(result == VK_SUCCESS);
    vk::vkGetBufferMemoryRequirements(*devicePointer, indexDatabuf, &imem_reqs);
    assert(idataByteCount<=imem_reqs.size);
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;
    alloc_info.allocationSize = imem_reqs.size;
    VkFlags requirements_mask=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bool flag=memoryTypeFromProperties(*memoryPropertiesPointer, imem_reqs.memoryTypeBits,requirements_mask,&alloc_info.memoryTypeIndex);
    if(flag)
    {
        LOGE("确定内存类型成功 类型索引为%d\n",alloc_info.memoryTypeIndex);
    }
    else
    {
        LOGE("确定内存类型失败!\n");
    }
    result = vk::vkAllocateMemory(*devicePointer,&alloc_info, NULL, &indexDataMem);
    assert(result == VK_SUCCESS);
    uint8_t *pData;
    result = vk::vkMapMemory(*devicePointer,indexDataMem, 0, imem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS);
    memcpy(pData, idata, idataByteCount);
    vk::vkUnmapMemory(*devicePointer, indexDataMem);
    result = vk::vkBindBufferMemory(*devicePointer,indexDatabuf, indexDataMem, 0);
    assert(result == VK_SUCCESS);
    indexDataBufferInfo.buffer = indexDatabuf;
    indexDataBufferInfo.offset = 0;
    indexDataBufferInfo.range = imem_reqs.size;
}
void BnggdhDraw::updateData(float time){ //更新动画数据的方法
    bnggdh->updata(time); //调用update 方法更新动画数据
    positionData = bnggdh->getPosition(); //获取更新后的顶点位置坐标数据
    textureData = bnggdh->getTextures(); //获取更新后的顶点纹理坐标数据
}
void BnggdhDraw::copyData(){ //拷贝顶点数据进入中间传输区的方法
    int count = 0; //辅助索引
    for (int i = 0;i<vCount;i++){ //遍历数组中的每一个元素
        vDataTransfer[count++] = positionData[i * 3 + 0]; //顶点位置坐标的X 分量
        vDataTransfer[count++] = positionData[i * 3 + 1]; //顶点位置坐标的Y 分量
        vDataTransfer[count++] = positionData[i * 3 + 2]; //顶点位置坐标的Z 分量
        vDataTransfer[count++] = textureData[i * 2 + 0]; //顶点的纹理S 坐标
        vDataTransfer[count++] = textureData[i * 2 + 1]; //顶点的纹理T 坐标
    }}
void BnggdhDraw::referVertexBuffer(){ //将顶点数据拷贝到绘制用内存的方法
    memcpy(vdata, vDataTransfer, vdataByteCount);//将顶点数据从中间传输区拷贝到绘制用内存区
}
void BnggdhDraw::drawSelf(string picName, //绘制方法
                          VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline){
    uint8_t *pData; //CPU 访问设备内存的辅助指针
    VkResult result = vk::vkMapMemory(*devicePointer, //将设备内存映射为CPU 可访问
                                      vertexDataMem, 0, vmem_reqs.size, 0, (void **)&pData);
    assert(result == VK_SUCCESS); //检查内存映射是否成功
    memcpy(pData, vdata, vdataByteCount); //将顶点数据拷贝进设备内存
    vk::vkUnmapMemory(*devicePointer, vertexDataMem); //解除内存映射
    VkDescriptorSet desSetPointer=MyVulkanManager::sqsCL->descSet[TextureManager::getVkDescriptorSetIndex(picName)];
    vk::vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline);
    vk::vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,&desSetPointer, 0, NULL);
    const VkDeviceSize offsetsVertex[1] = {0};
    vk::vkCmdBindVertexBuffers
            (
                    cmd,
                    0,
                    1,
                    &(vertexDatabuf),
                    offsetsVertex
            );
    vk::vkCmdBindIndexBuffer
            (
                    cmd,
                    indexDatabuf,
                    0,
                    VK_INDEX_TYPE_UINT16
            );
    float* mvp = MatrixState3D::getFinalMatrix();
    memcpy(pushConstantData, mvp, sizeof(float) * 16);
    vk::vkCmdPushConstants(cmd, pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 16, pushConstantData);
    vk::vkCmdDrawIndexed(cmd, iCount, 1, 0, 0, 0);
}
BnggdhDraw::~BnggdhDraw()
{
	delete bnggdh;
	delete[] positionData;
	delete[] textureData;
	delete[] pushConstantData;
    delete[] vdata;
    vk::vkDestroyBuffer(*devicePointer, vertexDatabuf, NULL);
    vk::vkFreeMemory(*devicePointer, vertexDataMem, NULL);
    vk::vkDestroyBuffer(*devicePointer, indexDatabuf, NULL);
    vk::vkFreeMemory(*devicePointer, indexDataMem, NULL);
}