#include "DoAction.h"

DoAction::DoAction(Robot *robotIn){
    robot=robotIn; //接收指向机器人对象的指针并保存
    ActionGenerator::genData(); //调用方法产生动作数据
    currAction=ActionGenerator::acVector[currActionIndex]; //获取当前动作
}
void DoAction::run(){
    robot->backToInit(); //调用backToInit 恢复机器人到初始状态
    if (currStep >= currAction->totalStep) { //若当前动作执行完毕，获取下一个动作
        currActionIndex=(currActionIndex+1)%ActionGenerator::acVector.size();//计算下一个动作的索引
        currAction = ActionGenerator::acVector[currActionIndex]; //获取下一个动作
        currStep = 0; //将当前动作已执行步骤数设置为0
    }
    for (float * ad:currAction->data) { //遍历数据中的每个组成部分
        int partIndex = (int) ad[0]; //取出部件索引
        int aType = (int) ad[1]; //取出动作类型
        if (aType == 0) { //若aType 为0，此部件动作为平移
            float xStart = ad[2];float yStart = ad[3];float zStart = ad[4]; //起始位置的x、y、z 坐标
            float xEnd = ad[5];float yEnd = ad[6];float zEnd = ad[7]; //结束位置的x、y、z 坐标
            //根据当前动作已执行的步骤数，线性插值计算出当前的平移数据
            float currX = xStart + (xEnd - xStart) * currStep / currAction->totalStep; //x 分量
            float currY = yStart + (yEnd - yStart) * currStep / currAction->totalStep; //y 分量
            float currZ = zStart + (zEnd - zStart) * currStep / currAction->totalStep; //z 分量
            //将当前部位的平移信息记录进此部位对应骨骼的变换矩阵
            robot->bpVector[partIndex]->translate(currX, currY, currZ);
        }
        else if (aType == 1) { //若aType 为1，此部件动作为旋转
            float startAngle = ad[2]; //旋转的起始角度
            float endAngle = ad[3]; //旋转的结束角度
            float currAngle = //根据当前动作已执行的步骤数，线性插值计算出当前的旋转角度
                    startAngle + (endAngle - startAngle) * currStep / currAction->totalStep;
            float x = ad[4];float y = ad[5]; float z = ad[6]; //取出此部位对应骨骼的旋转轴向量
            //将当前部位的旋转信息记录进此部位对应骨骼的变换矩阵
            robot->bpVector[partIndex]->rotate(currAngle, x, y, z);
        }}
    robot->updateState(); //调用updateState 方法逐层级联更新各层骨骼的变换矩阵
    currStep++; //当前动作已执行步骤数加1
}
DoAction::~DoAction()
{
}