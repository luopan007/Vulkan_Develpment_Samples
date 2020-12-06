#include <CameraUtil.h>
#include "TreeControl.h"
#include "ShaderQueueSuit_CommonTexBlend.h"
#include "MyVulkanManager.h"
#include <algorithm>
#include <MatrixState3D.h>
#include <Constant.h>
#include <HelpFunction.h>
#include "../util/Constant.h"
std::vector<TexLeafDrawableObject*> TreeControl::leafVector;//存放树叶vertor
std::vector<TexTrunkDrawableObject*> TreeControl::trunkVector;//存放树干的vertor
 std::vector<float*> TreeControl::leafInitialPos;//树叶初始位置
TreeControl::TreeControl(){
}
TreeControl::~TreeControl(){//释放相关占用内存
    for (std::vector<TexLeafDrawableObject *>::iterator it = leafVector.begin(); it != leafVector.end(); it ++){
        delete *it;
    }
    leafVector.clear();
    for (std::vector<TexTrunkDrawableObject *>::iterator it = trunkVector.begin(); it != trunkVector.end(); it ++){
        delete *it;
    }
    trunkVector.clear();
    for (std::vector<float*>::iterator it = leafInitialPos.begin(); it != leafInitialPos.end(); it ++){
        delete *it;
    }
    leafInitialPos.clear();
}
void TreeControl::drawSelf(float bend_RIn,float direction_degreeIn) {//绘制树的方法
    for(int i=0;i<trunkVector.size();i++){ //遍历树干部件列表
        trunkVector[i]->drawSelf(MyVulkanManager::cmdBuffer, //绘制当前遍历到的树干部件
                                 MyVulkanManager::sqsTrunk->pipelineLayout,MyVulkanManager::sqsTrunk->pipeline,
                                 &(MyVulkanManager::sqsTrunk->descSet[0]));
    }
    calculateCenterPosition(bend_RIn,direction_degreeIn);//计算树叶中心点坐标的方法
    calculateDistance();//根据树叶与摄像机距离进行排序
    for(int i=0;i<leafVector.size();i++){//绘制树叶
        float* position=calculateLastPosition(bend_RIn,direction_degreeIn,leafVector[i]->postionX,Constant::TREE_HEIGHT,leafVector[i]->postionZ);//计算树叶最终姿态
        MatrixState3D::pushMatrix();
        MatrixState3D::translate(position[0],position[1],position[2]);//进行平移
        MatrixState3D::rotate(position[5], -position[3],0,position[4]);//进行旋转
        leafVector[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsBlend->pipelineLayout, MyVulkanManager::sqsBlend->pipeline, &(MyVulkanManager::sqsBlend->descSet[0]));
        MatrixState3D::popMatrix();
    }
}
void TreeControl::calculateDistance(){
    sort(leafVector.begin(),leafVector.end(),compare);//对树叶列表中的树叶进行排序
}
bool TreeControl::compare(TexLeafDrawableObject *ta, TexLeafDrawableObject *another) {
    float xa=ta->postionX+ta->centerx-CameraUtil::cx;//第一个树叶与摄像机坐标的X 分量差值
    float ya=ta->postionY+ta->centery-CameraUtil::cy;//第一个树叶与摄像机坐标的Y 分量差值
    float za=ta->postionZ+ta->centerz-CameraUtil::cz;//第一个树叶与摄像机坐标的Z 分量差值
    float xb=another->postionX+another->centerx-CameraUtil::cx;//第二个树叶与摄像机坐标的X 分量差值
    float yb=another->postionY+another->centery-CameraUtil::cy;//第二个树叶与摄像机坐标的Y 分量差值
    float zb=another->postionZ+another->centerz-CameraUtil::cz;//第二个树叶与摄像机坐标的Z 分量差值
    float disA=xa*xa+ya*ya+za*za;//计算第一个树叶与摄像机之间距离的平方
    float disB=xb*xb+yb*yb+zb*zb;//计算第二个树叶与摄像机之间距离的平方
    return disA>disB; //返回两片树叶与摄像机距离值的比较结果
}
float* TreeControl::calculateLastPosition(float bend_R,float direction_degree,float pointX,float pointY,float pointZ){
    float* position=new float[6]; //记录位置、姿态数据的数组
    float curr_radian=pointY/bend_R; //计算当前的弧度
    float result_Y=(float) (bend_R*sin(curr_radian)); //计算结果的Y 分量
    float pianDistance=(float) (bend_R-bend_R*cos(curr_radian));//计算偏移距离(OD)
    float result_X=(float) (pointX+pianDistance*sin(toRadians(direction_degree)));//计算结果的X 分量
    float result_Z=(float)(pointZ+pianDistance*cos(toRadians(direction_degree))); //计算结果的Z 分量
    position[0]=result_X; //将计算出的X 分量存入结果数组
    position[1]=result_Y; //将计算出的Y 分量存入结果数组
    position[2]=result_Z; //将计算出的Z 分量存入结果数组
    position[3]=(float) cos(toRadians(direction_degree)); //计算旋转轴的X 分量
    position[4]=(float) sin(toRadians(direction_degree)); //计算旋转轴的Z 分量
    position[5]= (float)toDegrees(curr_radian); //计算旋转的角度
    return position; //返回位置、姿态数据数组

}
void TreeControl::calculateCenterPosition(float bend_RIn,float direction_degreeIn){
    //计算当前中心点坐标
    for(int i=0;i<leafVector.size();i++){//绘制树叶
        leafVector[i]->centerx=leafInitialPos[leafVector[i]->id][0]; //恢复初始中心点坐标X 分量(无风)
        leafVector[i]->centery=leafInitialPos[leafVector[i]->id][1]; //恢复初始中心点坐标Y 分量(无风)
        leafVector[i]->centerz=leafInitialPos[leafVector[i]->id][2]; //恢复初始中心点坐标Z 分量(无风)
        float curr_radian = leafVector[i]->centery / bend_RIn; //计算当前的弧度
        leafVector[i]->centery = (float) (bend_RIn * sin(curr_radian));//计算当前中心点Y 坐标
        float pianDistance = (float) (bend_RIn - bend_RIn * cos(curr_radian));//计算结果的偏移距离
        leafVector[i]->centerx = (float)(leafVector[i]->centerx + pianDistance * sin(toRadians(direction_degreeIn))); //计算当前中心点X 坐标
        leafVector[i]->centerz = (float) (leafVector[i]->centerz + pianDistance * cos(toRadians(direction_degreeIn))); //计算当前中心点Z 坐标
    }
}
