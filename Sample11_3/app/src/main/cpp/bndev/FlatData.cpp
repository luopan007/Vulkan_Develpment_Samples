#include "FlatData.h"
#include <vector>
#include <math.h>
#include <string.h>
#include <Constant.h>
float* FlatData::vdata;
int FlatData::dataByteCount;
int FlatData::vCount;
void  FlatData::genVertexData() {
    int cols = 31;
    int rows = cols;
    std::vector<float> alVertix;
    std::vector<float> alTexCoor;
    float widht = Constant::WATER_WIDTH;
    float UNIT_SIZE = widht/cols;
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            float zsx = -UNIT_SIZE*cols / 2 + i*UNIT_SIZE;
            float zsy = UNIT_SIZE*rows / 2 - j*UNIT_SIZE;
            float zsz = 0;
            alVertix.push_back(zsx);
            alVertix.push_back(zsy);
            alVertix.push_back(zsz);

            alVertix.push_back(zsx);
            alVertix.push_back(zsy - UNIT_SIZE);
            alVertix.push_back(zsz);

            alVertix.push_back(zsx + UNIT_SIZE);
            alVertix.push_back(zsy);
            alVertix.push_back(zsz);

            alVertix.push_back(zsx + UNIT_SIZE);
            alVertix.push_back(zsy);
            alVertix.push_back(zsz);

            alVertix.push_back(zsx);
            alVertix.push_back(zsy - UNIT_SIZE);
            alVertix.push_back(zsz);

            alVertix.push_back(zsx + UNIT_SIZE);
            alVertix.push_back(zsy - UNIT_SIZE);
            alVertix.push_back(zsz);
        }
    }
    for (int i = 0; i<cols; i++)
    {
        for (int j = 0; j<rows; j++)
        {
            alTexCoor.push_back(0);
            alTexCoor.push_back(0);
            alTexCoor.push_back(0);
            alTexCoor.push_back(1);
            alTexCoor.push_back(1);
            alTexCoor.push_back(0);
            alTexCoor.push_back(1);
            alTexCoor.push_back(0);
            alTexCoor.push_back(0);
            alTexCoor.push_back(1);
            alTexCoor.push_back(1);
            alTexCoor.push_back(1);
        }
    }
    vCount = cols*rows * 6;
    dataByteCount = vCount * 5 * sizeof(float);
    vdata = new float[vCount*5];
    int index = 0;
    for (int i = 0; i < vCount; i++) {
        vdata[index++] = alVertix[i * 3 + 0];
        vdata[index++] = alVertix[i * 3 + 1];
        vdata[index++] = alVertix[i * 3 + 2];
        vdata[index++] = alTexCoor[i * 2 + 0];
        vdata[index++] = alTexCoor[i * 2 + 1];
    }
}
