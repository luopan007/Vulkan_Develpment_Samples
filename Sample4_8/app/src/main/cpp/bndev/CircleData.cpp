#include "CircleData.h"
#include "BeltData.h"
#include <vector>
#include <math.h>
#include <string.h>

float* CircleData::vdata;
int CircleData::dataByteCount;
int CircleData::vCount;
void  CircleData::genVertexData(){
    int n = 10;												//扇形切割份数
    vCount=n+2;											//顶点数量
    dataByteCount=vCount*6* sizeof(float);						//顶点数据所占总字节数
    vdata=new float[vCount*6];									//创建顶点数据数组
    float angdegSpan=360.0f/n;								//扇形每份度数
    int count=0;											//辅助索引
    vdata[count++] = 0;										//第一个顶点X坐标
    vdata[count++] = 0;										//第一个顶点Y坐标
    vdata[count++] = 0;										//第一个顶点Z坐标
    vdata[count++] = 1;										//第一个顶点颜色R分量
    vdata[count++] = 1;										//第一个顶点颜色G分量
    vdata[count++] = 1;										//第一个顶点颜色B分量
    for(float angdeg=0; ceil(angdeg)<=360; angdeg+=angdegSpan) {	//循环生成周围其他顶点的数据
        double angrad=BeltData::toRadians(angdeg);				//当前弧度
        vdata[count++]=(float) (-50*sin(angrad));					//当前顶点x坐标
        vdata[count++]=(float) (50*cos(angrad));					//当前顶点y坐标
        vdata[count++]=0;									//当前顶点z坐标
        vdata[count++] = 0;									//当前顶点颜色R分量
        vdata[count++] = 1;									//当前顶点颜色G分量
        vdata[count++] = 0;									//当前顶点颜色B分量
}}
