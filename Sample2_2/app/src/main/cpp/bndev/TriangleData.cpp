#include "TriangleData.h"
#include <vector>
#include <math.h>
#include <string.h>

float *TriangleData::vdata;
int TriangleData::dataByteCount;
int TriangleData::vCount;

void TriangleData::genVertexData() {
    vCount = 3;
    dataByteCount = vCount * 6 * sizeof(float);
    vdata = new float[vCount * 6]{
            0, 75, 0, 1, 0, 0,
            -45, 0, 0, 0, 1, 0,
            45, 0, 0, 0, 0, 1
    };
}
