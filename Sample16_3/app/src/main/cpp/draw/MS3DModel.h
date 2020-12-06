#ifndef MS3DMODEL_H
#define MS3DMODEL_H
#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include "MS3DGroup.h"
#include <string>
#include <MS3DHeader.h>
#include <MS3DVertex.h>
#include <MS3DTriangle.h>
#include <MS3DGroup.h>
#include <MS3DMaterial.h>
#include <MS3DJoint.h>
#include <GLES/gl.h>
class MS3DModel{
public:
    MS3DHeader *header; //ms3d 文件头信息对象指针
    vector<MS3DVertex*> vertexs; //ms3d 文件顶点信息对象指针列表
    vector<MS3DTriangle*> triangles; //ms3d 文件三角形组装信息对象指针列表
    vector<MS3DGroup*> groups; //ms3d 文件组信息对象指针列表
    vector<MS3DMaterial*> materials; //ms3d 文件材质信息对象指针列表
    vector<MS3DJoint*> joints; //ms3d 文件关节信息对象指针列表
    int vCount; //顶点数量
    int groCount; //组数量
    int joiCount; //关节数量
    float fps; //ms3d 文件帧速率信息
    float current_time; //ms3d 文件当前播放时间信息
    float totalTime; //总动画时间
    int frame_count; //ms3d 文件关键帧数量信息
    MS3DJoint* jointHelper; //更新顶点数据所需的指向当前所用关节对象的指针
    float * pushConstantData; //指向推送常量数据数组首地址的指针
    vector<float*> vdata; //指向组内顶点数据数组首地址的指针列表
    VkDevice* devicePointer; //指向逻辑设备的指针
    float* groVdata; //指向绘制用当前组顶点数据数组首地址的指针
    VkBuffer *vertexDatabuf; //指向顶点数据缓冲数组首地址的指针
    VkDeviceMemory *vertexDataMem; //指向顶点数据所需设备内存数组首地址的指针
    VkDescriptorBufferInfo *vertexDataBufferInfo; //指向顶点数据缓冲描述信息数组首地址的指针
    VkMemoryRequirements *mem_reqs; //指向缓冲内存需求数组首地址的指针
    MS3DModel(VkDevice& device, //构造函数
              VkPhysicalDeviceMemoryProperties& memoryroperties, unsigned char* binaryData);
    ~MS3DModel(); //析构函数
    void initMs3dInfo(unsigned char* binaryData); //加载ms3d 文件信息的方法
    void initVertexBuffer(VkPhysicalDeviceMemoryProperties&
    memoryroperties); //创建顶点数据缓冲的方法
    void updateJoint(float ttime); //更新关节数据的方法
    void updateAllVectexs(); //更新所有顶点位置数据的方法
    void initVertexData(); //初始化所有顶点数据的方法
    void animate(float ttime,VkCommandBuffer& cmd,
                 VkPipelineLayout& pipelineLayout,VkPipeline& pipeline); //执行动画的方法
    void reAssemVertexData(); //更新顶点数据后重新组装数据的方法
    void drawSelf(bool isUpdate,VkCommandBuffer& cmd, //绘制方法
                  VkPipelineLayout& pipelineLayout,VkPipeline& pipeline);
};
#endif
