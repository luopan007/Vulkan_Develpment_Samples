#include "TriangleData.h"
#include <vector>
#include <math.h>
#include <string.h>

float* TriangleData::vdata;
int TriangleData::dataByteCount;
int TriangleData::vCount;
void TriangleData::genVertexData(){
    vCount = 6; //顶点数量
    dataByteCount=vCount*6* sizeof(float); //顶点数据所占总字节数
    vdata=new float[vCount*6]{ //初始化顶点数据数组
            -90,60,0, 1,1,1, //左侧三角形第一个顶点数据
            -90,-60,0, 0,1,0, //左侧三角形第二个顶点数据
            -30,-60,0, 1,1,1, //左侧三角形第三个顶点数据
            30,60,0, 1,1,1, //右侧三角形第一个顶点数据
            90,60,0, 1,1,1, //右侧三角形第二个顶点数据
            90,-60,0, 0,1,0, //右侧三角形第三个顶点数据
    };}

