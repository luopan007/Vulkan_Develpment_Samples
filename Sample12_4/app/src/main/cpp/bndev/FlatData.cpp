#include "FlatData.h"
#include <vector>
#include <math.h>
#include <string.h>
/*
 *     2       12'
 *
 *
 *      33'    1'
 *
 * */
float* FlatData::vdata;
int FlatData::dataByteCount;
int FlatData::vCount;
void  FlatData::genVertexData() {
    const float width=3;
    const float height=3;
    vCount = 6; //顶点数量为6
    dataByteCount = vCount * 5 * sizeof(float); //数据所占内存总字节数
    vdata = new float[vCount * 5]{ //顶点数据数组
            width, height, 0, 1, 0,
            -width, height, 0, 0, 0,
            -width, -height, 0, 0, 1,
            width, -height, 0, 1, 1,
            width, height, 0, 1, 0,
            -width, -height, 0, 0, 1
    };
}
