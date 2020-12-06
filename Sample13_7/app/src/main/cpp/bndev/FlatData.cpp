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
    const float width = 1.0f;
    const float height =1.0f;
    vCount = 6;
    dataByteCount = vCount * 5 * sizeof(float);
    vdata = new float[vCount * 5]{
            width, height, 0, 1, 0,
            -width, height, 0, 0, 0,
            -width, -height, 0, 0, 1,
            width, -height, 0, 1, 1,
            width, height, 0, 1, 0,
            -width, -height, 0, 0, 1
    };
}
