#include "FlatData.h"
#include <vector>
#include <math.h>
#include <string.h>
float* FlatData::vdata; //数据数组首地址指针
int FlatData::dataByteCount; //数据所占总字节数
int FlatData::vCount; //顶点数量
void FlatData::genData(){ //顶点数据生成方法
    vCount = 6; //顶点数量为6
    dataByteCount = vCount * 6 * sizeof(float); //数据所占内存总字节数
    vdata = new float[vCount * 6]{ //顶点数据数组
            3, 2, 0, 0, 0, 1, -3, 2, 0, 0, 0, 1, //每个顶点6 个数据
            -3, -2, 0, 0, 0, 1, 3, -2, 0, 0, 0, 1, //前3 个是顶点位置坐标XYZ 分量
            3, 2, 0, 0, 0, 1, -3, -2, 0, 0, 0, 1}; //后3 个是顶点法向量XYZ 分量
}