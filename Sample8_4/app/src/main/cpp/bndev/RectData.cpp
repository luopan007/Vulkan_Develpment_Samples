#include "RectData.h"
float RectData::span = 10;
float* RectData::vdata;
int RectData::dataByteCount;
int RectData::vCount;
void RectData::genVertexData()
{
    vCount = 6; 
    dataByteCount = vCount * 6 * sizeof(float);
    vdata = new float[vCount * 6]
            {
                    -span,span,0, 1,1,1,
                    -span,-span,0, 1,1,1,
                    span,span,0, 1,1,1,
                    -span,-span,0, 1,1,1,
                    span,-span,0, 1,1,1,
                    span,span,0, 1,1,1,
            };
}
