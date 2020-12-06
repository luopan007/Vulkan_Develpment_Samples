#ifndef ROBOT_H
#define ROBOT_H
#include "MatrixState3D.h"
#include "ObjObject.h"
#include "BodyPart.h"
using namespace std;
class Robot
{
public:
    BodyPart                                    
                *bRoot,*bBody,*bHead,
                *bLeftTop,*bLeftBottom,*bRightTop,*bRightBottom,
                *bRightLegTop,*bRightLegBottom,*bLeftLegTop,*bLeftLegBottom,
                *bLeftFoot,*bRightFoot;
    std::vector<BodyPart*> bpVector;             
    float lowest=MAXFLOAT;
    Robot(vector<ObjObject*> objObject);
    ~Robot();
    void updateState();                         
    void backToInit();                          
    void calLowest();
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);        
};
#endif
