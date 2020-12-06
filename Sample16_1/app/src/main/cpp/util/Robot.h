#ifndef ROBOT_H
#define ROBOT_H
#include "MatrixState3D.h"
#include "ObjObject.h"
#include "BodyPart.h"
using namespace std;
class Robot{
public:
    BodyPart *bRoot,*bBody,*bHead, *bLeftTop, //指向机器人模型身体各个部位对象的指针
            *bLeftBottom,*bRightTop,*bRightBottom, *bRightLegTop, *bRightLegBottom,
            *bLeftLegTop,*bLeftLegBottom, *bLeftFoot, *bRightFoot;
    std::vector<BodyPart*> bpVector; //指向机器人身体各个部位对象的指针列表
    Robot(vector<ObjObject*> objObject); //构造函数
    ~Robot(); //析构函数
    void updateState(); //更新机器人状态的方法
    void backToInit(); //恢复机器人姿态到初始状态的方法
    void drawSelf(VkCommandBuffer& cmd, //绘制机器人的方法
                  VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
};
#endif
