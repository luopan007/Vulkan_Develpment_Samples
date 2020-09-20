#include <BeltData.h>
#include <vector>
#include <math.h>
#include <string.h>
#include "BeltData.h"

const double PI=3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170680;

float* BeltData::vdata;
int BeltData::dataByteCount;
int BeltData::vCount;

double BeltData::toRadians(double d)
{
    double e=d*PI/180;
    return e;
}
void  BeltData::genVertexData(){								//生成顶点数据的方法
    int n = 6;                              						//条带切割份数
    vCount=2*(n+1);                							//计算顶点数量
    dataByteCount=vCount*6* sizeof(float);						//计算顶点数据所占总字节数
    float angdegBegin = -90;									//设置条带起始度数
    float angdegEnd = 90;     								//设置条带结束度数
    float angdegSpan = (angdegEnd-angdegBegin)/n;				//计算条带每份度数
    vdata=new float[vCount*6];									//创建顶点数据数组
    int count=0;											//辅助索引
    for(float angdeg=angdegBegin; angdeg<=angdegEnd; angdeg+=angdegSpan) {
        double angrad=toRadians(angdeg);						//计算当前弧度
        vdata[count++]=(float) (-0.6f*50*sin(angrad));			//此切割角度第1个顶点的x坐标
        vdata[count++]=(float) (0.6f*50*cos(angrad));			//此切割角度第1个顶点的y坐标
        vdata[count++]=0;								//此切割角度第1个顶点的z坐标
        vdata[count++]=1;								//此切割角度第1个顶点颜色R分量
        vdata[count++]=1;								//此切割角度第1个顶点颜色G分量
        vdata[count++]=1;								//此切割角度第1个顶点颜色B分量
        vdata[count++]=(float) (-50*sin(angrad));				//此切割角度第2个顶点的x坐标
        vdata[count++]=(float) (50*cos(angrad));				//此切割角度第2个顶点的y坐标
        vdata[count++]=0;								//此切割角度第2个顶点的z坐标
        vdata[count++]=0;								//此切割角度第2个顶点颜色R分量
        vdata[count++]=1;								//此切割角度第2个顶点颜色G分量
        vdata[count++]=1;								//此切割角度第2个顶点颜色B分量
}}

