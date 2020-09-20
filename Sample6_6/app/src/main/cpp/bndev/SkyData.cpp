#include "SkyData.h"
#include <vector>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define random() (rand()%30000)/30000.0f //随机函数宏
#define PI 3.141592654 //PI 的值
#define UNIT_SIZE 2000 //单位尺寸
float* SkyData::vdata; //顶点数据数组首地址指针
int SkyData::dataByteCount; //顶点数据所占总字节数
int SkyData::vCount; //顶点数量
void SkyData::genSkyData(int vCountIn){ //生成星星顶点数据
    vCount = vCountIn; //顶点数量
    dataByteCount = vCount * 6 * sizeof(float); //顶点数据所占总字节数
    vdata = new float[vCount * 6]; //创建存放顶点数据的数组
    for (int i = 0; i<vCount; i++){ //遍历每颗星星（即每个顶点）
        double angleTempJD = PI * 2 * random(); //随机产生星星的经度
        double angleTempWD = PI*(random() - 0.5f); //随机产生星星的纬度
        vdata[i * 6 + 0] = float(UNIT_SIZE*cos(angleTempWD)*sin(angleTempJD)); //顶点位置 X 分量
        vdata[i * 6 + 1] = float(UNIT_SIZE*sin(angleTempWD)); //顶点位置 Y 分量
        vdata[i * 6 + 2] = float(UNIT_SIZE*cos(angleTempWD)*cos(angleTempJD)); //顶点位置 Z 分量
        vdata[i * 6 + 3] = 1.0; //颜色值 R 分量
        vdata[i * 6 + 4] = 1.0; //颜色值 G 分量
        vdata[i * 6 + 5] = 1.0; //颜色值 B 分量
    }}

