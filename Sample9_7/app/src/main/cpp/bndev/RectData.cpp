#include "RectData.h"
float RectData::span = 10;
float* RectData::vData;
int RectData::dataByteCount;
int RectData::vCount;
void RectData::getVertexData()
{
    vCount = 6;
    dataByteCount = vCount * 6 * sizeof(float);
    vData = new float[vCount * 6]
            {
                    -span,span,0,     0,0,
                    -span,-span,0,    0,1,
                    span,span,0,      1,0,
                    -span,-span,0,    0,1,
                    span,-span,0,     1,1,
                    span,span,0,      1,0
            };
}
