#include "VertData.h"
#include <vector>
#include <math.h>
#include <string.h>
#include <assert.h>
float* VertData::vdata;
int VertData::dataByteCount;
int VertData::vCount;
int* VertData::indexData;
int VertData::indexDataByteCount;
int VertData::indexCount;
float toRadians(float degree)
{
    return float(degree*3.1415926535898 / 180);
}
float* generateTexCoor(int bw, int bh)
{
    float* result = new float[(bw + 1)*(bh + 1) * 2];
    float sizew = 1.0f / bw;
    float sizeh = 1.0f / bh;
    int c = 0;
    for (int i = 0; i <= bh; i++)
    {
        for (int j = 0; j <= bw; j++)
        {
            float s = j*sizew;
            float t = i*sizeh;
            result[c++] = s;
            result[c++] = t;
        }
    }
    return result;
}
void  VertData::genVertData()
{
    float UNIT_SIZE = WIDTH_SPAN / (CR - 1);
    float xStart = -WIDTH_SPAN / 2;
    float zStart = -WIDTH_SPAN / 2;
    std::vector<float> alVertix;
    for (int i = 0; i <= CR; i++)
    {
        for (int j = 0; j <= CR; j++)
        {
            float x1 = xStart + j*UNIT_SIZE;
            float z1 = zStart + i*UNIT_SIZE;
            float y1 = 0;
            alVertix.push_back(x1); alVertix.push_back(y1); alVertix.push_back(z1);
        }
    }
    float* texCoor = generateTexCoor(
            CR, 
            CR  
    );
    vCount = alVertix.size() / 3;
    dataByteCount = vCount * 8 * sizeof(float);
    vdata = new float[vCount * 8];
    int index = 0;
    int texIndex = 0;
    for (int i = 0; i<vCount; i++)
    {
        vdata[index++] = alVertix[i * 3 + 0];
        vdata[index++] = alVertix[i * 3 + 1];
        vdata[index++] = alVertix[i * 3 + 2];
        vdata[index++] = texCoor[texIndex++];
        vdata[index++] = texCoor[texIndex++];
        vdata[index++] = 0;
        vdata[index++] = 1;
        vdata[index++] = 0;
    }
    indexCount = CR*CR * 6;
    indexData = new int[indexCount];
    int tempCount = 0;
    for (int i = 0; i<CR; i++)
    {
        for (int j = 0; j<CR; j++)
        {
            int widthTemp = CR + 1;
            int index0 = j*widthTemp + i;
            int index1 = index0 + 1;
            int index2 = index0 + 1 + widthTemp;
            int index3 = index0 + widthTemp;
            indexData[tempCount * 6] = index0;
            indexData[tempCount * 6 + 1] = index3;
            indexData[tempCount * 6 + 2] = index1;
            indexData[tempCount * 6 + 3] = index1;
            indexData[tempCount * 6 + 4] = index3;
            indexData[tempCount * 6 + 5] = index2;
            tempCount++;
        }
    }
    indexDataByteCount = indexCount * 4;
}