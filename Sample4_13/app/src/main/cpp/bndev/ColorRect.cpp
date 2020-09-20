#include "ColorRect.h"

#define UINT_SIZE 600

float* ColorRect::vdataG;
float* ColorRect::vdataR;
int ColorRect::dataByteCount;
int ColorRect::vCount;

void  ColorRect::genVertexData()
{
    vCount=6;
    dataByteCount=vCount*6* sizeof(float);
    vdataG=new float[vCount*6]{
            0,0,0,                        0,1,1,
            UINT_SIZE,UINT_SIZE,0,        0,1,1,
            -UINT_SIZE,UINT_SIZE,0,       0,1,1,
            -UINT_SIZE,-UINT_SIZE,0,      0,1,1,
            UINT_SIZE,-UINT_SIZE,0,       0,1,1,
            UINT_SIZE,UINT_SIZE,0,        0,1,1
    };//青绿色顶点数据数组

    vdataR=new float[vCount*6]{
            0,0,0,                        1,1,0,
            UINT_SIZE,UINT_SIZE,0,        1,1,0,
            -UINT_SIZE,UINT_SIZE,0,       1,1,0,
            -UINT_SIZE,-UINT_SIZE,0,      1,1,0,
            UINT_SIZE,-UINT_SIZE,0,       1,1,0,
            UINT_SIZE,UINT_SIZE,0,        1,1,0
    };//粉红色顶点坐标数据数组

}






