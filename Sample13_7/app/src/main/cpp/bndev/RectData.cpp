#include "RectData.h"
#include "Constant.h"
float* RectData::vData;
int RectData::dataByteCount;
int RectData::vCount;
void RectData::getVertexData()
{
    vCount = 6;
    dataByteCount = vCount * 6 * sizeof(float);
    vData = new float[vCount * 6]{
            -Constant::UNIT_SIZE, Constant::UNIT_SIZE, 0,1,0,
            -Constant::UNIT_SIZE, -Constant::UNIT_SIZE, 0, 1,1,
            Constant::UNIT_SIZE, -Constant::UNIT_SIZE, 0, 0,1,
            Constant::UNIT_SIZE, -Constant::UNIT_SIZE, 0,0,1,
            Constant::UNIT_SIZE, Constant::UNIT_SIZE, 0,0,0,
            -Constant::UNIT_SIZE, Constant::UNIT_SIZE, 0,1,0
    };
}

