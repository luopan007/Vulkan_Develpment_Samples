//
// Created by Administrator on 2017/10/UINT_SIZE.
//

#include <MatrixState3D.h>
#include <DrawableObjectCommon.h>
#include "Cube.h"
#include "mylog.h"

#define UINT_SIZE 30 //立方体边长
DrawableObjectCommon *colorRect; //指向绘制对象（正方形面物体）的指针
Cube::Cube(VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties){//构造函数
    ColorRect::genVertexData(); //生成正方形顶点数据
    colorRect=new DrawableObjectCommon(ColorRect::vdata, //创建绘制对象(正方形
                                       ColorRect::dataByteCount,ColorRect::vCount,device,memoryroperties);

}
void Cube::drawSelf(VkCommandBuffer cmd, //绘制方法
                    VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer){
    MatrixState3D::pushMatrix(); //保护现场
    MatrixState3D::translate(0,0,UINT_SIZE); //平移到相应位置
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer); //绘制前面正方形
    MatrixState3D::popMatrix();

    //后
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0,0,-UINT_SIZE);
    MatrixState3D::rotate(180,0,1,0);
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    MatrixState3D::popMatrix();

    //左
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(-UINT_SIZE,0,0);
    MatrixState3D::rotate(-90,0,1,0);
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    MatrixState3D::popMatrix();

    //右
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(UINT_SIZE,0,0);
    MatrixState3D::rotate(90,0,1,0);
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    MatrixState3D::popMatrix();

    //上
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0,UINT_SIZE,0);
    MatrixState3D::rotate(-90,1,0,0);
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    MatrixState3D::popMatrix();

    //下
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0,-UINT_SIZE,0);
    MatrixState3D::rotate(90,1,0,0);
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    MatrixState3D::popMatrix();

}
Cube::~Cube(){ //析构函数
    delete colorRect; //销毁绘制对象(正方形面)
}

