#include <cmath>
#include "BallForControl.h"
#include "CollisionUtil.h"
#include "MyVulkanManager.h"

Vector3f* BallForControl::tmpAxis= new Vector3f();
Quaternion* BallForControl::tmpQuaternion = new Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
Vector3f* BallForControl::axis = new Vector3f();

BallForControl::BallForControl(ObjObject* obj, float xOffset, float zOffset,string textureName){
this->obj = obj; //接收指向绘制对象的指针并保存
this->xOffset = xOffset; //接收桌球初始位置坐标的X 分量并保存
this->zOffset = zOffset; //接收桌球初始位置坐标的Z 分量并保存
this->textureName = textureName; //接收桌球使用的纹理名称并保存
}
void BallForControl::drawSelf( //绘制方法
        VkCommandBuffer& cmd, ShaderQueueSuit_Common* sqsCL){
    MatrixState3D::pushMatrix(); //保护现场
    MatrixState3D::translate(xOffset, BALL_R, zOffset); //平移到指定位置
    MatrixState3D::scale(BALL_SCALE, BALL_SCALE, BALL_SCALE); //执行缩放变换
//绕旋转轴旋转(旋转轴垂直于运动方向，平行于桌面)
    if (abs(angleCurr) != 0 && (abs(currAxisX) != 0 || abs(currAxisY) != 0 || abs(currAxisZ) != 0)){
        MatrixState3D::rotate(angleCurr, currAxisX, currAxisY, currAxisZ); //执行旋转变换
    }
    obj->drawSelf(cmd, sqsCL->pipelineLayout, sqsCL->pipeline, //绘制当前桌球
                  &(sqsCL->descSet[TextureManager::getVkDescriptorSetIndex(textureName)]));
    MatrixState3D::popMatrix(); //恢复现场
}

float toDegrees(float angrad)
{
	return angrad * 180.0f / 3.141592653589793f;
}

void BallForControl::go(){ //根据桌球当前速度向前运动一步的方法
    float vTotal = (float)sqrt(vx*vx + vz*vz); //计算桌球当前总速度标量
    if (vTotal == 0){return;} //若速度为0 则直接返回，不进行运动模拟
    float tempX = xOffset; //拷贝桌球此步运动前位置坐标的X 分量
    float tempZ = zOffset; //拷贝桌球此步运动前位置坐标的Z 分量
    xOffset = xOffset + vx*TIME_SPAN; //根据速度计算桌球下一步位置坐标的X 分量
    zOffset = zOffset + vz*TIME_SPAN; //根据速度计算桌球下一步位置坐标的Z 分量
    bool flag = false; //记录桌球是否发生碰撞的标志，false 表示未碰撞
    for (int i = 0; i<MyVulkanManager::allBall.size(); i++){//遍历所有桌球判断是否相互发生碰撞
        BallForControl* bfcL = MyVulkanManager::allBall[i]; //获取当前需要判断的桌球对象
        if (bfcL != this){ //若当前需要判断的球不是自身
            bool tempFlag = CollisionUtil::collisionCalculate(this, bfcL);//计算两球的碰撞情况
            if (tempFlag) flag = true; //根据碰撞情况设置标志位
        }}
    if (xOffset<-TABLE_SIZE + BALL_R || xOffset>TABLE_SIZE - BALL_R){ //碰左挡板或右挡板
        vx = -vx; //速度的X 分量置反
        flag = true; //碰撞标志置true
    }else if (zOffset<-TABLE_SIZE + BALL_R || zOffset>TABLE_SIZE - BALL_R){//碰前挡板或后挡板
        vz = -vz; //速度的Z 分量置反
        flag = true; //碰撞标志置true
    }
    if (flag == false){ //若桌球未发生碰撞
        float distance = (float)vTotal*TIME_SPAN; //计算桌球此步运动的距离
        tmpAxis->x = vz; tmpAxis->y = 0; tmpAxis->z = -vx; //设置本次旋转轴向量
        float tmpAngrad = distance / (BALL_R); //根据运动的距离计算球需要转动的角度
        tmpQuaternion->setToRotateAboutAxis( //通过旋转轴和旋转角设置四元数各分量
                tmpAxis, tmpAngrad);
        quaternionTotal->cross(tmpQuaternion, quaternionTotal);//将临时四元数叉乘总的四元数
        quaternionTotal->getRotationAxis(axis); //提取总旋转轴
        float angrad = quaternionTotal->getRotationAngle(); //获取当前总的旋转角
        currAxisX=axis->x;currAxisY=axis->y;currAxisZ=axis->z; //设置当前总旋转轴的各个分量
        angleCurr = (float)toDegrees(angrad); //设置当前的总旋转角
        vx = vx*V_TENUATION; //每次运动后衰减速度X 分量
        vz = vz*V_TENUATION; //每次运动后衰减速度Z 分量
    }else{ //若桌球发生了碰撞
        xOffset = tempX; zOffset = tempZ; //本次桌球位置不变，即不产生移动
    }
    if (vTotal<V_THRESHOLD){ //当桌球总速度小于阈值
        vx = 0;vz = 0; //设置桌球速度为0，使其停止
        return;
    }}

BallForControl::~BallForControl()
{
	delete obj;
	delete axis;
	delete tmpAxis;
	delete tmpQuaternion;
	delete quaternionTotal;
}