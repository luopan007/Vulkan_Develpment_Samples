//
// Created by Administrator on 2017/10/UINT_SIZE.
//

#include <MatrixState3D.h>
#include <DrawableObjectCommon.h>
#include "Cube.h"
#include "mylog.h"

Cube::Cube(VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties,float *vdata,float unit_sizeIn)
{
    unit_size=unit_sizeIn;
    colorRect=new DrawableObjectCommon(vdata,ColorRect::dataByteCount,ColorRect::vCount,device,memoryroperties);
}

void Cube::drawSelf(VkCommandBuffer cmd,VkPipelineLayout& pipelineLayout,VkPipeline& pipeline,VkDescriptorSet* desSetPointer)
{

    //绘制前小面
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0, 0, unit_size);
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    MatrixState3D::popMatrix();

    //绘制后小面
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0, 0, -unit_size);
    MatrixState3D::rotate(180, 0, 1, 0);
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    MatrixState3D::popMatrix();

    //绘制上大面
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0,unit_size,0);
    MatrixState3D::rotate(-90, 1, 0, 0);
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    MatrixState3D::popMatrix();

    //绘制下大面
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0,-unit_size,0);
    MatrixState3D::rotate(90, 1, 0, 0);
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    MatrixState3D::popMatrix();

    //绘制左大面
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(unit_size,0,0);
    MatrixState3D::rotate(-90, 1, 0, 0);
    MatrixState3D::rotate(90, 0, 1, 0);
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    MatrixState3D::popMatrix();

    //绘制右大面
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(-unit_size,0,0);
    MatrixState3D::rotate(90, 1, 0, 0);
    MatrixState3D::rotate(-90, 0, 1, 0);
    colorRect->drawSelf(cmd,pipelineLayout,pipeline,desSetPointer);
    MatrixState3D::popMatrix();
}
Cube::~Cube()
{
    delete colorRect;
}

