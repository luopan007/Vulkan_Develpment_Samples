#ifndef SAMPLE7_5_SCBARANGETHREADTASK_H
#define SAMPLE7_5_SCBARANGETHREADTASK_H

#include <vulkan/vulkan.h>

class SCBArangeThreadTask {
public :
    VkCommandBuffer secondary_cmds[1]; //二级命令缓冲
    VkCommandBufferBeginInfo* secondary_begin; //二级命令缓冲启动信息
    VkCommandPool cmdPool; //命令池
    VkDevice* device; //指向设备的指针
    bool doTaskFlag = false; //当前线程任务完成标志
    float* currMatrix; //服务于一个线程的基本变换矩阵
    float* MVP; //服务于一个线程的最终变换矩阵
    SCBArangeThreadTask(uint32_t queueGraphicsFamilyIndex,VkDevice* device, VkCommandBufferBeginInfo* secondary_begin); //构造函数
    ~SCBArangeThreadTask(); //析构函数
    void initBeforeDraw(); //绘制任务前准备二级命令缓冲的方法
    void closeAfterDraw(); //绘制任务后结束二级命令缓冲的方法

};
#endif //SAMPLE7_5_SCBARANGETHREADTASK_H
