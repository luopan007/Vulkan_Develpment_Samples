#include <MyVulkanManager.h>
#include "Matrix.h"
#include "MatrixState3D.h"
#include "TextureManager.h"
BodyPart::BodyPart(float fxIn,float fyIn,float fzIn,ObjObject * objForDrawIn,int indexIn){//构造函数
    this->index=indexIn; //接收绘制用最终矩阵索引并保存
    this->objForDraw=objForDrawIn; //接收物体绘制类对象指针并保存
    this->fx=fxIn; this->fy=fyIn; this->fz=fzIn; //接收此骨骼不动点在世界坐标系中的原始坐标并保存
    mFather=new float[16]; //初始化骨骼在父骨骼坐标系中的实时变换矩阵对应数组
    mWorld=new float[16]; //初始化骨骼在世界坐标系中的实时变换矩阵对应数组
    mFatherInit=new float[16]; //初始化骨骼在父骨骼坐标系中的初始变换矩阵对应数组
    mWorldInitInver=new float[16]; //初始化骨骼在世界坐标系中的初始变化矩阵的逆矩阵数组
    mFinal=new float[16]; //初始化最终变换矩阵对应数组
}
void BodyPart::initFratherMatrix(){ //初始化骨骼在父骨骼坐标系中的初始变换矩阵
    float tx=fx;float ty=fy; float tz=fz; //复制此骨骼关节点在世界坐标系中的x、y、z 坐标
    if(father!=NULL){ //若父骨骼不为空
        tx=fx-father->fx; ty=fy-father->fy;tz=fz-father->fz; //计算子骨骼在父骨骼坐标系中的原始坐标
    }
    Matrix::setIdentityM(mFather, 0); //初始化此骨骼在父骨骼坐标系中的实时变换矩阵
    Matrix::translateM(mFather, 0, tx, ty, tz); //将平移信息记录进此矩阵
    memcpy(mFatherInit,mFather,16* sizeof(float));//将数据复制进骨骼在父骨骼坐标系中的初始变换矩阵
    for(BodyPart* bpc:children){ //循环初始化所有子骨骼的相关矩阵
        bpc->initFratherMatrix(); //调用子骨骼的initFratherMatrix 方法
    }}
void BodyPart::calMWorldInitInver(){ //计算此骨骼在世界坐标系中初始变换矩阵逆矩阵的方法
    Matrix::invertM(mWorldInitInver, 0, mWorld, 0); //计算自身的所需逆矩阵
    for(BodyPart* bpc:children) //循环对所有子骨骼进行计算
        bpc->calMWorldInitInver(); //调用子骨骼的calMWorldInitInver 方法
}
void BodyPart::updateBone(){ //层次级联更新骨骼变换矩阵的方法
    if(father!=NULL){ //若父骨骼不为空，则此骨骼在世界坐标系中的变换矩阵为自身矩阵乘以父骨骼矩阵
        Matrix::multiplyMM(mWorld, 0, father->mWorld, 0, mFather, 0);
    }else{ //若父骨骼为空，则此骨骼在世界坐标系中的变换矩阵为自己的变换矩阵
        memcpy(mWorld,mFather,16* sizeof(float)); //复制矩阵元素
    }
    calFinalMatrix(); //调用calFinalMatrix 方法计算最终变换矩阵
    for(BodyPart* bpc:children){ //循环更新所有子骨骼的变换矩阵
        bpc->updateBone(); //调用子骨骼的updateBone 方法
    }}
void BodyPart::calFinalMatrix(){ //计算最终变换矩阵的方法
    Matrix::multiplyMM(mFinal, 0, mWorld, 0, mWorldInitInver, 0);
}
void BodyPart::backToInit(){ //恢复骨骼姿态到初始状态的方法
    memcpy(mFather,mFatherInit,16* sizeof(float)); //复制骨骼在父骨骼坐标系中的初始变换矩阵元素
    for(BodyPart* bpc:children){bpc->backToInit(); //循环对所有子骨骼执行恢复动作
    }}
void BodyPart::translate(float x,float y,float z){ //骨骼自身平移的方法
    Matrix::translateM(mFather, 0, x, y, z); //将平移信息记录进相应矩阵
}
void BodyPart::rotate(float angle,float x,float y,float z){ //骨骼自身旋转的方法
    Matrix::rotateM(mFather,0,angle,x,y,z); //将旋转信息记录进相应矩阵
}
void BodyPart::addChild(BodyPart*child){ //添加子骨骼的方法
    this->children.push_back(child); //添加子骨骼
    child->father=this; //设置父骨骼
}
void BodyPart::drawSelf(VkCommandBuffer& cmd, //绘制部位自身的方法
                        VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer) {
    if (index == 2) { //判断当前部位是否为头部
        desSetPointer = &(MyVulkanManager:: //设置指向绘制头部所需的描述集指
        sqsCL->descSet[TextureManager::getVkDescriptorSetIndex("texture/head.bntex")]);
    }
    if (objForDraw != NULL) {
        MatrixState3D::pushMatrix(); //保护现场
        MatrixState3D::setMatrix(mFinal); //设置基本变换矩阵
        objForDraw->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer); //绘制对应部位
        MatrixState3D::popMatrix(); //恢复现场
    }
    for (BodyPart *bpc:children) { //循环绘制所有子部位
        bpc->drawSelf(cmd, pipelineLayout, pipeline, desSetPointer); //调用子部位的绘制方法
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