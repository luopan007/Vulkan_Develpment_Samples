#ifndef BODYPART_H
#define BODYPART_H
#include "ObjObject.h"
#include "Robot.h"
#include <vector>
using namespace std;
class BodyPart
{
public :
    ObjObject * objForDraw;          
    int index;                      
    float * mFather;                
    float * mWorld;                 
    float * mFatherInit;            
    float * mWorldInitInver;        
    float * mFinal;                 
    float fx,fy,fz;                 
    std::vector<BodyPart*> children; 
    BodyPart * father=NULL;              
    BodyPart(float fxIn,float fyIn,float fzIn,ObjObject * objForDrawIn,int indexIn);
    BodyPart(float fxIn,float fyIn,float fzIn,ObjObject * objForDrawIn,int indexIn,bool lowestFlagIn,vector<float*>lowestDotsIn);
    ~BodyPart();
    /*===================一系列计算方法======================*/
    void initFratherMatrix();       
    void calMWorldInitInver();      
    void updateBone();              
    void calFinalMatrix();          
    void backToInit();              
    void translate(float x,float y,float z);                            
    void rotate(float angle,float x,float y,float z);                   
    void addChild(BodyPart * child);  
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
    bool  lowestFlag;
    vector<float*> lowestDots;
    float* pqc = new float[4]();
    float* resultP = new float[4]();
    void calLowest();
};
#endif
