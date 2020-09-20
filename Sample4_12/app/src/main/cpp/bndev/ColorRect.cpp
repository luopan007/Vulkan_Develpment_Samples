
#include "ColorRect.h"
#include <vector>
#include <math.h>
#include <string.h>
float ColorRect::UNIT_SIZEG=500; //青色正方形半边长
float ColorRect::UNIT_SIZEY=499.5; //黄色正方形半边长
float* ColorRect::vdataG; //青色正方形顶点数据数组首地址指针
float* ColorRect::vdataY; //黄色正方形顶点数据数组首地址指针
int ColorRect::dataByteCount; //每个正方形顶点数据所占总字节数
int ColorRect::vCount; //每个正方形顶点数量
void ColorRect::genVertexData(){ //生成顶点数据的方法
    vCount=6; //顶点数量
    dataByteCount=vCount*6* sizeof(float); //每个正方形顶点数据所占总字节数
    vdataG=new float[vCount*6]{ //青色正方形顶点数据数组
            0,0,0, 0,1,1, //第1 个点的位置和颜色数据
            UNIT_SIZEG,UNIT_SIZEG,0, 0,1,1, //第2 个点的位置和颜色数据
            -UNIT_SIZEG,UNIT_SIZEG,0, 0,1,1, //第3 个点的位置和颜色数据
            -UNIT_SIZEG,-UNIT_SIZEG,0, 0,1,1, //第4 个点的位置和颜色数据
            UNIT_SIZEG,-UNIT_SIZEG,0, 0,1,1, //第5 个点的位置和颜色数据
            UNIT_SIZEG,UNIT_SIZEG,0, 0,1,1 //第6 个点的位置和颜色数据
    };
    vdataY=new float[vCount*6]{
            0,0,0,                        1,1,0,
            UNIT_SIZEY,UNIT_SIZEY,0,        1,1,0,
            -UNIT_SIZEY,UNIT_SIZEY,0,       1,1,0,
            -UNIT_SIZEY,-UNIT_SIZEY,0,      1,1,0,
            UNIT_SIZEY,-UNIT_SIZEY,0,       1,1,0,
            UNIT_SIZEY,UNIT_SIZEY,0,        1,1,0
    };//粉红色顶点坐标数据数组

}