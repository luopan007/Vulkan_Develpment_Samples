#include "RectData.h"
float RectData::span = 60;
float* RectData::vData;
int RectData::vCount;
int RectData::dataByteCount;
void RectData::getVertexData()
{
    vCount = 6;
    dataByteCount = vCount * 5 * sizeof(float);
    vData = new float[vCount * 5]
            {
                  span,0,-span,     6,0,
                    -span,0,-span,  0,0,
                    -span,0,span,   0,6,
                    -span,0,span,   0,6,
                    span,0,span,    6,6,
                    span,0,-span,   6,0
            };
}
