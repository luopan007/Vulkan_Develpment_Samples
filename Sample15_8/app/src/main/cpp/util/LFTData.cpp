#include "LFTData.h"
float* LFTData::vdata;
int LFTData::dataByteCount;
int LFTData::vCount;
void LFTData::getVData(){
    vCount = 36;
    dataByteCount = sizeof(float) * vCount * 6;
    vdata = new float[vCount * 6]
            {
                    0.0*CUBE_W, 0.0*CUBE_H, 1.0*CUBE_D,      0.0, 0.0, 1.0,
                    0.0*CUBE_W, 0.0*CUBE_H, 0.0*CUBE_D,      0.0, 0.0, 0.0,
                    1.0*CUBE_W, 0.0*CUBE_H, 0.0*CUBE_D,      1.0, 0.0, 0.0,
                    1.0*CUBE_W, 0.0*CUBE_H, 0.0*CUBE_D,      1.0, 0.0, 0.0,
                    1.0*CUBE_W, 0.0*CUBE_H, 1.0*CUBE_D,      1.0, 0.0, 1.0,
                    0.0*CUBE_W, 0.0*CUBE_H, 1.0*CUBE_D,      0.0, 0.0, 1.0,
                    0.0*CUBE_W, 1.0*CUBE_H, 1.0*CUBE_D,      0.0, 1.0, 1.0,
                    1.0*CUBE_W, 1.0*CUBE_H, 1.0*CUBE_D,      1.0, 1.0, 1.0,
                    1.0*CUBE_W, 1.0*CUBE_H, 0.0*CUBE_D,      1.0, 1.0, 0.0,
                    1.0*CUBE_W, 1.0*CUBE_H, 0.0*CUBE_D,      1.0, 1.0, 0.0,
                    0.0*CUBE_W, 1.0*CUBE_H, 0.0*CUBE_D,      0.0, 1.0, 0.0,
                    0.0*CUBE_W, 1.0*CUBE_H, 1.0*CUBE_D,      0.0, 1.0, 1.0,
                    0.0*CUBE_W, 0.0*CUBE_H, 1.0*CUBE_D,      0.0, 0.0, 1.0,
                    1.0*CUBE_W, 0.0*CUBE_H, 1.0*CUBE_D,      1.0, 0.0, 1.0,
                    1.0*CUBE_W, 1.0*CUBE_H, 1.0*CUBE_D,      1.0, 1.0, 1.0,
                    1.0*CUBE_W, 1.0*CUBE_H, 1.0*CUBE_D,      1.0, 1.0, 1.0,
                    0.0*CUBE_W, 1.0*CUBE_H, 1.0*CUBE_D,      0.0, 1.0, 1.0,
                    0.0*CUBE_W, 0.0*CUBE_H, 1.0*CUBE_D,      0.0, 0.0, 1.0,
                    1.0*CUBE_W, 0.0*CUBE_H, 1.0*CUBE_D,      1.0, 0.0, 1.0,
                    1.0*CUBE_W, 0.0*CUBE_H, 0.0*CUBE_D,      1.0, 0.0, 0.0,
                    1.0*CUBE_W, 1.0*CUBE_H, 0.0*CUBE_D,      1.0, 1.0, 0.0,
                    1.0*CUBE_W, 1.0*CUBE_H, 0.0*CUBE_D,      1.0, 1.0, 0.0,
                    1.0*CUBE_W, 1.0*CUBE_H, 1.0*CUBE_D,      1.0, 1.0, 1.0,
                    1.0*CUBE_W, 0.0*CUBE_H, 1.0*CUBE_D,      1.0, 0.0, 1.0,
                    1.0*CUBE_W, 0.0*CUBE_H, 0.0*CUBE_D,      1.0, 0.0, 0.0,
                    0.0*CUBE_W, 0.0*CUBE_H, 0.0*CUBE_D,      0.0, 0.0, 0.0,
                    0.0*CUBE_W, 1.0*CUBE_H, 0.0*CUBE_D,      0.0, 1.0, 0.0,
                    0.0*CUBE_W, 1.0*CUBE_H, 0.0*CUBE_D,      0.0, 1.0, 0.0,
                    1.0*CUBE_W, 1.0*CUBE_H, 0.0*CUBE_D,      1.0, 1.0, 0.0,
                    1.0*CUBE_W, 0.0*CUBE_H, 0.0*CUBE_D,      1.0, 0.0, 0.0,
                    0.0*CUBE_W, 0.0*CUBE_H, 0.0*CUBE_D,      0.0, 0.0, 0.0,
                    0.0*CUBE_W, 0.0*CUBE_H, 1.0*CUBE_D,      0.0, 0.0, 1.0,
                    0.0*CUBE_W, 1.0*CUBE_H, 1.0*CUBE_D,      0.0, 1.0, 1.0,
                    0.0*CUBE_W, 1.0*CUBE_H, 1.0*CUBE_D,      0.0, 1.0, 1.0,
                    0.0*CUBE_W, 1.0*CUBE_H, 0.0*CUBE_D,      0.0, 1.0, 0.0,
                    0.0*CUBE_W, 0.0*CUBE_H, 0.0*CUBE_D,      0.0, 0.0, 0.0
            };
}
