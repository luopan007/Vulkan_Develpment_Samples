#include "Robot.h"
using namespace std;
Robot::Robot(vector<ObjObject*> objObject)
{
    bRoot=new BodyPart(0.0f,0.0f,0.0f,NULL,0); //创建根骨骼对应部位对象
    bBody=new BodyPart(0.0f,0.938f,0.0f,objObject[0],1); //创建身体对应部位对象
    bHead=new BodyPart(0.0f,1.00f,0.0f,objObject[1],2);
    bLeftTop=new BodyPart(0.107f,0.938f,0.0f,objObject[2],3);
    bLeftBottom=new BodyPart(0.105f,0.707f,-0.033f,objObject[3],4);
    bRightTop=new BodyPart(-0.107f,0.938f,0.0f,objObject[4],5);
    bRightBottom=new BodyPart(-0.105f,0.707f,-0.033f,objObject[5],6);
    bRightLegTop=new BodyPart(-0.068f,0.6f,0.02f,objObject[6],7);
    bRightLegBottom=new BodyPart(-0.056f,0.312f,0.0f,objObject[7],8);
    bLeftLegTop=new BodyPart(0.068f,0.6f,0.02f,objObject[8],9);
    bLeftLegBottom=new BodyPart(0.056f,0.312f,0.0f,objObject[9],10);
    bLeftFoot=new BodyPart(0.068f,0.038f,0.033f,objObject[10],11);
    bRightFoot=new BodyPart(-0.068f,0.038f,0.033f,objObject[11],12);
    bpVector.push_back(bRoot); //将根骨骼对象指针存入列表
    bpVector.push_back(bBody); //将身体部位对象指针存入列表
    bpVector.push_back(bHead);
    bpVector.push_back(bLeftTop);
    bpVector.push_back(bLeftBottom);
    bpVector.push_back(bRightTop);
    bpVector.push_back(bRightBottom);
    bpVector.push_back(bRightLegTop);
    bpVector.push_back(bRightLegBottom);
    bpVector.push_back(bLeftLegTop);
    bpVector.push_back(bLeftLegBottom);
    bpVector.push_back(bLeftFoot);
    bpVector.push_back(bRightFoot);
    bRoot->addChild(bBody); //设置机器人身体部位为根骨骼的子部位
    bBody->addChild(bHead); //设置机器人头部为身体的子部位
    bBody->addChild(bLeftTop);
    bBody->addChild(bRightTop);
    bLeftTop->addChild(bLeftBottom);
    bRightTop->addChild(bRightBottom);
    bBody->addChild(bRightLegTop);
    bRightLegTop->addChild(bRightLegBottom);
    bBody->addChild(bLeftLegTop);
    bLeftLegTop->addChild(bLeftLegBottom);
    bLeftLegBottom->addChild(bLeftFoot);
    bRightLegBottom->addChild(bRightFoot);
   bRoot->initFratherMatrix(); //调用initFatherMatrix 方法初始化相关矩阵
   bRoot->updateBone(); //调用updateBone 方法计算骨骼在世界坐标系中的初始变换矩阵
   bRoot->calMWorldInitInver(); //调用calMWorldInitInver 方法计算初始变换矩阵的逆矩阵
}
void Robot::updateState(){ //更新机器人状态的方法
    bRoot->updateBone(); //调用根骨骼的updateBone 方法逐层级联更新所有骨骼的变换矩阵
}
void Robot::backToInit(){ //恢复机器人姿态到初始状态的方法
    bRoot->backToInit(); //调用根骨骼的backToInit 方法逐层级联恢复所有骨骼到初始状态
}
void Robot::drawSelf(VkCommandBuffer& cmd, //绘制方法
                     VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer){
    MatrixState3D::pushMatrix(); //保护现场
    bRoot->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);//调用根骨骼的绘制方法逐层级联绘制
    MatrixState3D::popMatrix(); //恢复现场
}
Robot::~Robot()
{
    for(BodyPart* bp:bpVector)
    {
        delete bp;
    }
}
