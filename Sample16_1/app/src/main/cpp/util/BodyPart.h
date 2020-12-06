#ifndef BODYPART_H
#define BODYPART_H
#include "ObjObject.h"
#include "Robot.h"
#include <vector>
using namespace std;
class BodyPart{ 
public :
    ObjObject * objForDraw; 
    int index;
    float * mFather; //指向此骨骼在父骨骼坐标系中的实时变换矩阵对应数组首地址的指针
    float * mWorld; //指向此骨骼在世界坐标系中的实时变换矩阵对应数组首地址的指针
    float * mFatherInit; //指向此骨骼在父骨骼坐标系中的初始变换矩阵对应数组首地址的指针
    float * mWorldInitInver; //指向此骨骼在世界坐标系中初始变换矩阵的逆矩阵对应数组首地址的指针
    float * mFinal; //指向最终变换矩阵对应数组首地址的指针
    float fx,fy,fz; //此骨骼不动点在世界坐标系中的原始坐标
    std::vector<BodyPart*> children; //此骨骼的直接子骨骼列表
    BodyPart * father=NULL; //指向父骨骼对象的指针
    BodyPart(float fxIn,float fyIn,float fzIn,ObjObject * objForDrawIn,int indexIn); //构造函数
    ~BodyPart(); //析构函数
    void initFratherMatrix(); //初始化各相关矩阵的方法
    void calMWorldInitInver(); //计算此骨骼在世界坐标系中初始变换矩阵逆矩阵的方法
    void updateBone(); //层次级联更新骨骼变换矩阵的方法
    void calFinalMatrix(); //计算最终变换矩阵的方法
    void backToInit(); //恢复骨骼姿态到初始状态的方法
    void translate(float x,float y,float z); //骨骼自身平移的方法
    void rotate(float angle,float x,float y,float z);//骨骼自身旋转的方法
    void addChild(BodyPart * child); //添加子骨骼的方法
    void drawSelf(VkCommandBuffer& cmd,VkPipelineLayout& pipelineLayout, //绘制方法
                  VkPipeline& pipeline,VkDescriptorSet* desSetPointer);
};
#endif
