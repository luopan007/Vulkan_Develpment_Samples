
#include "ColorRect.h"
#include <vector>
#include <math.h>
#include <string.h>

float* ColorRect::vdata;
int ColorRect::dataByteCount;
int ColorRect::vCount;
void ColorRect::genVertexData(){ //生成顶点数据的方法
    vCount=6; //顶点数量
    dataByteCount=vCount*6* sizeof(float); //顶点数据总字节数
    vdata=new float[vCount*6]{ //顶点数据数组
            0,0,0, 1,1,1, //第1 个顶点的位置及颜色数据
            30,30,0, 0,0,1, //第2 个顶点的位置及颜色数据
            -30,30,0, 0,0,1, //第3 个顶点的位置及颜色数据
            -30,-30,0, 0,0,1, //第4 个顶点的位置及颜色数据
            30,-30,0, 0,0,1, //第5 个顶点的位置及颜色数据
            30,30,0, 0,0,1 //第6 个顶点的位置及颜色数据
    };}
