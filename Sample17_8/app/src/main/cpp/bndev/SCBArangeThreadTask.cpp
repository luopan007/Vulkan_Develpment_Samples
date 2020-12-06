#include "SCBArangeThreadTask.h"
#include "MyVulkanManager.h"
#include <thread>
#include <assert.h>

SCBArangeThreadTask::SCBArangeThreadTask(uint32_t queueGraphicsFamilyIndex, VkDevice* device, VkCommandBufferBeginInfo* secondary_begin)
{
    this->secondary_begin = secondary_begin; //接收二级命令缓冲启动信息
    this->device = device; //接收指定的设备
    VkCommandPoolCreateInfo cmd_pool_info = {}; //构建命令池创建信息结构体实例
    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_info.pNext = NULL; //自定义数据的指针
    cmd_pool_info.queueFamilyIndex = queueGraphicsFamilyIndex;//绑定队列家族索引
    cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VkResult result =vk::vkCreateCommandPool(*device, &cmd_pool_info, NULL, &cmdPool);
    assert(result == VK_SUCCESS); //检查命令池是否创建成功
    VkCommandBufferAllocateInfo cmdalloc = {}; //构建命令缓冲分配信息结构体实例
    cmdalloc.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdalloc.pNext = NULL; //自定义数据的指针
    cmdalloc.commandPool = cmdPool; //绑定命令池
    cmdalloc.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY; //分配的命令缓冲级别
    cmdalloc.commandBufferCount = 1; //分配的命令缓冲数量
    result =vk::vkAllocateCommandBuffers(*device, &cmdalloc, secondary_cmds);//分配命令缓冲
    assert(result == VK_SUCCESS); //检测分配是否成功
    currMatrix=new float[16]; //创建服务于一个线程的基本变换矩阵数组
    MVP = new float[16]; //创建服务于一个线程的最终变换矩阵数组
}

SCBArangeThreadTask::~SCBArangeThreadTask()
{
    vk::vkFreeCommandBuffers(*device, cmdPool, 1, secondary_cmds); //释放二级命令缓冲
    vk::vkDestroyCommandPool(*device, cmdPool, NULL); //销毁命令池
    delete[] currMatrix; //释放服务于一个线程的基本变换矩阵数组
    delete[] MVP; //释放服务于一个线程的最终变换矩阵数组
}

void SCBArangeThreadTask::initBeforeDraw()
{
    vk::vkResetCommandBuffer(secondary_cmds[0], 0); //重置二级命令缓冲
    vk::vkBeginCommandBuffer(secondary_cmds[0], secondary_begin);//启动二级命令缓冲
}

void SCBArangeThreadTask::closeAfterDraw()
{
    vk::vkEndCommandBuffer(secondary_cmds[0]); //结束二级命令缓冲
}