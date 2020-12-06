#include "Robot.h"
using namespace std;
Robot::Robot(vector<ObjObject*> objObject)
{
    bRoot=new BodyPart(0.0f,0.0f,0.0f,NULL,0);  
    bBody=new BodyPart(0.0f,0.938f,0.0f,objObject[0],1);
    bHead=new BodyPart(0.0f,1.00f,0.0f,objObject[1],2);
    bLeftTop=new BodyPart(0.107f,0.938f,0.0f,objObject[2],3);
    bLeftBottom=new BodyPart(0.105f,0.707f,-0.033f,objObject[3],4);
    bRightTop=new BodyPart(-0.107f,0.938f,0.0f,objObject[4],5);
    bRightBottom=new BodyPart(-0.105f,0.707f,-0.033f,objObject[5],6);
    bRightLegTop=new BodyPart(-0.068f,0.6f,0.02f,objObject[6],7);
    bRightLegBottom=new BodyPart(-0.056f,0.312f,0.0f,objObject[7],8);
    bLeftLegTop=new BodyPart(0.068f,0.6f,0.02f,objObject[8],9);
    bLeftLegBottom=new BodyPart(0.056f,0.312f,0.0f,objObject[9],10);
    vector<float*> lFoot; //存储左脚最低控制点原始坐标的列表
    lFoot.push_back((new float[3]{0.068f,0.0f,0.113f})); //将第1 组左脚最低点控制数据存入列表
    lFoot.push_back(new float[3]{0.068f,0.0f,-0.053f}); //将第2 组左脚最低点控制数据存入列表
    bLeftFoot=new BodyPart(0.068f,0.038f,0.033f,objObject[10],11, true,lFoot);//创建左脚对应部位
    vector<float*> rFoot; //存储右脚最低控制点原始坐标的列表
    rFoot.push_back(new float[3]{-0.068f,0.0f,0.113f}); //将第1 组右脚最低点控制数据存入列表
    rFoot.push_back(new float[3]{-0.068f,0.0f,-0.053f}); //将第2 组右脚最低点控制数据存入列表
    bRightFoot=new BodyPart(-0.068f,0.038f,0.033f,objObject[11],12,true,rFoot);//创建右脚对应部位
    bpVector.push_back(bRoot);
    bpVector.push_back(bBody);
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
    bRoot->addChild(bBody);
    bBody->addChild(bHead);
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
    bRoot->initFratherMatrix();         
    bRoot->updateBone();                
    bRoot->calMWorldInitInver();        
}
void Robot::updateState() 
{
    bRoot->updateBone();
}
void Robot::backToInit(){	
    bRoot->backToInit();
}
void Robot::calLowest() { //用于计算机器人模型最低点的方法
    lowest=MAXFLOAT; //将最低点y 坐标设置为浮点数最大值
    bRoot->calLowest(); //调用根部位的calLowest 方法层次级联计算出实际最低点的Y 坐标
}
void Robot::drawSelf(VkCommandBuffer& cmd, //绘制方法
                     VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer) {
    MatrixState3D::pushMatrix(); //保护现场
    MatrixState3D::translate(0, -lowest, 0); //执行平移变换以微调模型位置
    bRoot->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer); //绘制根骨骼对应的部位
    MatrixState3D::popMatrix(); //恢复现场
}
Robot::~Robot()
{
    for(BodyPart* bp:bpVector)
    {
        delete bp;
    }
}
