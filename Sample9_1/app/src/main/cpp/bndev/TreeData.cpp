#include "TreeData.h"
float TreeData::width;
float TreeData::height;
float* TreeData::vData;
int TreeData::dataByteCount;
int TreeData::vCount;
void TreeData::getVertexData()
{
    width = 6;
    height = 15;
    vCount = 6;
    dataByteCount = vCount * 5 * sizeof(float);
    vData = new float[vCount * 5]
            {
                    width, 0, 0,        1,1,
                    width,height,0,     1,0,
                    -width,height,0,    0,0,
                    -width,height,0,    0,0,
                    -width,0,0,         0,1,
                    width,0,0,          1,1
            };
}
