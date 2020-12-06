#ifndef DOACTION_H
#define DOACTION_H
#include "Action.h"
#include "Robot.h"
#include "ActionGenerator.h"
class DoAction{
    int currActionIndex=0; //当前动作的编号
    int currStep=0; //当前动作已执行的步骤数
    Action *currAction; //当前动作的数据
    Robot * robot; //指向所控制机器人的指针
public:
    DoAction(Robot * robot); //构造函数
    void run(); //用于更新骨骼动画的方法
    ~DoAction(); //析构函数
};
#endif
