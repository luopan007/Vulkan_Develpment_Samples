#ifndef DOACTION_H
#define DOACTION_H
#include "Action.h"
#include "Robot.h"
#include "ActionGenerator.h"
class DoAction
{
    int currActionIndex=0;          
    int currStep=0;                 
    Action *currAction;             
    Robot * robot;                  
    ActionGenerator* actionGen;     
public:
    DoAction(Robot * robot);
    void run();
    ~DoAction();
};
#endif
