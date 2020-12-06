#include "BallForControl.h"
BallForControl::BallForControl(float startYIn)
{
    this->startY=startYIn;
    this->currentY=startYIn;
}
BallForControl::~BallForControl(){}
void BallForControl::step()
{
    timeLive+=TIME_SPAN;
    float tempCurrY=startY-0.5f*G*timeLive*timeLive+vy*timeLive;
    if(tempCurrY<=0)
    {
        startY=0;
        vy=-(vy-G*timeLive)*0.995f;
        timeLive=0;
        if(vy<0.35f)
        {
            currentY=0;
        }
    }
    else
    {
        currentY=tempCurrY;
    }
}