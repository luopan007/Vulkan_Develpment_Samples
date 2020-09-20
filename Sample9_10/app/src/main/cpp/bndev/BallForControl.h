#ifndef VULKANEXBASE_BALLFORCONTROL_H
#define VULKANEXBASE_BALLFORCONTROL_H
#define TIME_SPAN 0.02f
#define G 5.0f
class BallForControl
{
public:
    float startY;
    float timeLive=0;
    float currentY=0;
    float vy=0;
    BallForControl(float startYIn);
    ~BallForControl();
    void step();
};
#endif
