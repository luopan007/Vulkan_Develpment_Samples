#include "RectData.h"
float* RectData::vData;
int RectData::dataByteCount;
int RectData::vCount;
void RectData::getVertexData()
{
    vCount = 6;
    dataByteCount = vCount * 6 * sizeof(float);
    vData = new float[vCount * 6]
            {
                    - SKY_BOX_HALF_EDGE,SKY_BOX_HALF_EDGE,0,     0,0,
                    -SKY_BOX_HALF_EDGE,-SKY_BOX_HALF_EDGE,0,    0,1,
                    SKY_BOX_HALF_EDGE,SKY_BOX_HALF_EDGE,0,      1,0,

                    -SKY_BOX_HALF_EDGE,-SKY_BOX_HALF_EDGE,0,    0,1,
                    SKY_BOX_HALF_EDGE,-SKY_BOX_HALF_EDGE,0,     1,1,
                    SKY_BOX_HALF_EDGE,SKY_BOX_HALF_EDGE,0,      1,0
            };
}

