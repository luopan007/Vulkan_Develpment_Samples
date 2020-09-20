#include "SkyData.h"
#include <vector>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define random() (rand()%30000)/30000.0f
#define PI  3.141592654
#define UNIT_SIZE 2000
float* SkyData::vdata;
int SkyData::dataByteCount;
int SkyData::vCount;
void  SkyData::genSkyData(int vCountIn)
{
    vCount = vCountIn;
    dataByteCount = vCount * 6 * sizeof(float);
    vdata = new float[vCount * 6];
    for (int i = 0; i<vCount; i++)
    {
        double angleTempJD = PI * 2 * random();
        double angleTempWD = PI*(random() - 0.5f);
        vdata[i * 6 + 0] = float(UNIT_SIZE*cos(angleTempWD)*sin(angleTempJD));
        vdata[i * 6 + 1] = float(UNIT_SIZE*sin(angleTempWD));
        vdata[i * 6 + 2] = float(UNIT_SIZE*cos(angleTempWD)*cos(angleTempJD));
        vdata[i * 6 + 3] = 1.0;
        vdata[i * 6 + 4] = 1.0;
        vdata[i * 6 + 5] = 1.0;
    }
}
