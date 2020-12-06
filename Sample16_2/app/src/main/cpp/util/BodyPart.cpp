#include <MyVulkanManager.h>
#include "Matrix.h"
#include "MatrixState3D.h"
#include "TextureManager.h"
BodyPart::BodyPart(float fxIn,float fyIn,float fzIn,ObjObject * objForDrawIn,int indexIn)
{
    this->index=indexIn;
    this->objForDraw=objForDrawIn;
    this->fx=fxIn;              
    this->fy=fyIn;              
    this->fz=fzIn;              
    mFather=new float[16];          
    mWorld=new float[16];           
    mFatherInit=new float[16];      
    mWorldInitInver=new float[16];  
    mFinal=new float[16];           
}

BodyPart::BodyPart(float fxIn,float fyIn,float fzIn, //构造函数
                   ObjObject * objForDrawIn,int indexIn,bool lowestFlagIn,vector<float*>lowestDotsIn){
    this->index=indexIn;
    this->objForDraw=objForDrawIn;
    this->fx=fxIn;
    this->fy=fyIn;
    this->fz=fzIn;
    mFather=new float[16];
    mWorld=new float[16];
    mFatherInit=new float[16];
    mWorldInitInver=new float[16];
    mFinal=new float[16];
    this->lowestFlag=lowestFlagIn; //接收是否有最低控制点的标志并保存
    this->lowestDots=lowestDotsIn; //接收最低控制点列表并保存
}
void BodyPart::initFratherMatrix()       
{
    float tx=fx;                
    float ty=fy;                
    float tz=fz;                
    if(father!=NULL)
    {                           
        tx=fx-father->fx;       
        ty=fy-father->fy;       
        tz=fz-father->fz;       
    }
    Matrix::setIdentityM(mFather, 0);            
    Matrix::translateM(mFather, 0, tx, ty, tz);  
    memcpy(mFatherInit,mFather,16* sizeof(float));           
    for(BodyPart* bpc:children)
    {
        bpc->initFratherMatrix(); 
    }
}
void BodyPart::calMWorldInitInver()             
{
    Matrix::invertM(mWorldInitInver, 0, mWorld, 0);
    for(BodyPart* bpc:children)
    {
        bpc->calMWorldInitInver();  
    }
}
void BodyPart::updateBone()                     
{
    if(father!=NULL)                            
    {
        Matrix::multiplyMM(mWorld, 0, father->mWorld, 0, mFather, 0);
    }
    else
    {
        memcpy(mWorld,mFather,16* sizeof(float));           
    }
    calFinalMatrix();
    for(BodyPart* bpc:children)
    {
        bpc->updateBone();                      
    }
}
void BodyPart::calLowest(){ //级联计算最低控制点的方法
    if(lowestFlag) { //判断当前部位是否有最低控制点
        for (float *p:lowestDots) { //循环对每一个最低控制点进行计算
            pqc[0]=p[0];pqc[1]=p[1];pqc[2]=p[2];pqc[3]=1; //该点的初始坐标
            resultP[0]=0; resultP[1]=0; resultP[2]=0; resultP[3]=1; //重置resultP 所指向数组的元素
            Matrix::multiplyMV(resultP, 0, mFinal, 0, pqc, 0); //计算变换后的坐标
            if (resultP[1]<MyVulkanManager::robot->lowest){ //如果该点y 坐标小于当前模型最低点y 坐标
                MyVulkanManager::robot->lowest = resultP[1]; //更新机器人模型的最低点y 坐标
            }}}
    for(BodyPart* bp:children){ //对所有的子部位进行相同的计算
        bp->calLowest();
    }}
void BodyPart::calFinalMatrix(){                 
    Matrix::multiplyMM(mFinal, 0, mWorld, 0, mWorldInitInver, 0);
}
void BodyPart::backToInit(){                     
    memcpy(mFather,mFatherInit,16* sizeof(float));
    for(BodyPart* bpc:children){bpc->backToInit();                      
    }}
void BodyPart::translate(float x,float y,float z){                            
    Matrix::translateM(mFather, 0, x, y, z);
}
void BodyPart::rotate(float angle,float x,float y,float z){                   
    Matrix::rotateM(mFather,0,angle,x,y,z);
}
void BodyPart::addChild(BodyPart*child){  
    this->children.push_back(child);
    child->father=this;
}
void BodyPart::drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout,
                        VkPipeline& pipeline,VkDescriptorSet* desSetPointer){
    if(index==2){
        desSetPointer=&(MyVulkanManager::
        sqsCL->descSet[TextureManager::getVkDescriptorSetIndex("texture/head.bntex")]);
    }
    if (objForDraw != NULL) {
        MatrixState3D::pushMatrix();
        MatrixState3D::setMatrix(mFinal);   
        objForDraw->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer);
        MatrixState3D::popMatrix();
    }
    for(BodyPart* bpc:children){
        bpc->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    }
}
BodyPart::~BodyPart()
{
    delete objForDraw;
    delete father;
    delete [] mFather;
    delete [] mWorld;
    delete [] mFatherInit;
    delete [] mWorldInitInver;
    delete [] mFinal;
}