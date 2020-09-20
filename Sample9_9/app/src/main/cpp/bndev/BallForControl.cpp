#include "BallForControl.h"
BallForControl::BallForControl(float startYIn) { //构造函数
    this->startY=startYIn; //初始化起始位置
    this->currentY=startYIn; //初始化当前位置
}
BallForControl::~BallForControl(){} //析构函数
void BallForControl::step() { //物理计算的具体实现方法
    timeLive+=TIME_SPAN; //时间递增
    float tempCurrY=startY-0.5f*G*timeLive*timeLive+vy*timeLive; //计算当前高度(牛顿定律)
    if(tempCurrY<=0) { //如果当前位置低于地面则反弹
        startY=0; //反弹后起始位置置0
        vy=-(vy-G*timeLive)*0.995f; //反弹后初始速度
        timeLive=0; //反弹后此轮时间置0
        if(vy<0.35f) { //如果速度小于阈值则停止运动
            currentY=0; } //恢复起始位置
    } else { //若没有碰到地面则正常运动
        currentY = tempCurrY; //更新当前位置
    }}