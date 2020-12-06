#include "DoAction.h"
DoAction::DoAction(Robot *robotIn)
{
    robot=robotIn;
    ActionGenerator::genData();
    currAction=ActionGenerator::acVector[currActionIndex];	
}
DoAction::~DoAction()
{
}
void DoAction::run()
{
    robot->backToInit();
    if (currStep >= currAction->totalStep) {
        currActionIndex = (currActionIndex + 1) % ActionGenerator::acVector.size();
        currAction = ActionGenerator::acVector[currActionIndex];
        currStep = 0;
    }
    for (float * ad:currAction->data) {
        int partIndex = (int) ad[0];
        int aType = (int) ad[1]; 
        if (aType == 0) {
            float xStart = ad[2];
            float yStart = ad[3];
            float zStart = ad[4];
            float xEnd = ad[5];
            float yEnd = ad[6];
            float zEnd = ad[7];
            float currX = xStart + (xEnd - xStart) * currStep / currAction->totalStep;
            float currY = yStart + (yEnd - yStart) * currStep / currAction->totalStep;
            float currZ = zStart + (zEnd - zStart) * currStep / currAction->totalStep;
            robot->bpVector[partIndex]->translate(currX, currY, currZ);
        }
        else if (aType == 1) {
            float startAngle = ad[2];
            float endAngle = ad[3];
            float currAngle =
                    startAngle + (endAngle - startAngle) * currStep / currAction->totalStep;
            float x = ad[4];
            float y = ad[5];
            float z = ad[6];
            robot->bpVector[partIndex]->rotate(currAngle, x, y, z);
        }
    }
    robot->updateState();
    robot->calLowest(); //调用calLowest 方法逐层级联计算最低控制点
    currStep++;  
}
