#include "BallData.h"
#include <vector>
#include <math.h>
#include <string.h>

float* BallData::vdata;
int BallData::dataByteCount;
int BallData::vCount;
float toRadians(float degree)
{
    return degree*3.1415926535898/180;
}
void  BallData::genBallData(float angleSpan)
{
    const float r = 1.0f;
    std::vector<float> alVertix;
    std::vector<float> alLongLat;
    for (float vAngle = -90; vAngle < 90; vAngle = vAngle + angleSpan) {
        for (float hAngle = 0; hAngle <= 360; hAngle = hAngle + angleSpan) {
            float x0 = (float)(r * cos(toRadians(vAngle)) * cos(toRadians(hAngle)));
            float y0 = (float)(r * cos(toRadians(vAngle)) * sin(toRadians(hAngle)));
            float z0 = (float)(r *sin(toRadians(vAngle)));
            float long0 = hAngle; float lat0 = vAngle;
            float x1 = (float)(r * cos(toRadians(vAngle)) * cos(toRadians(hAngle + angleSpan)));
            float y1 = (float)(r * cos(toRadians(vAngle)) *sin(toRadians(hAngle + angleSpan)));
            float z1 = (float)(r * sin(toRadians(vAngle)));
            float long1 = hAngle + angleSpan; float lat1 = vAngle;
            float x2 = (float)(r*cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle + angleSpan)));
            float y2 = (float)(r*cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle + angleSpan)));
            float z2 = (float)(r * sin(toRadians(vAngle + angleSpan)));
            float long2 = hAngle + angleSpan; float lat2 = vAngle + angleSpan;
            float x3 = (float)(r * cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle)));
            float y3 = (float)(r * cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle)));
            float z3 = (float)(r * sin(toRadians(vAngle + angleSpan)));
            float long3 = hAngle; float lat3 = vAngle + angleSpan;
            alVertix.push_back(x1); alVertix.push_back(y1); alVertix.push_back(z1);
            alVertix.push_back(x3); alVertix.push_back(y3); alVertix.push_back(z3);
            alVertix.push_back(x0); alVertix.push_back(y0); alVertix.push_back(z0);
            alVertix.push_back(x1); alVertix.push_back(y1); alVertix.push_back(z1);
            alVertix.push_back(x2); alVertix.push_back(y2); alVertix.push_back(z2);
            alVertix.push_back(x3); alVertix.push_back(y3); alVertix.push_back(z3);
            alLongLat.push_back(long1); alLongLat.push_back(lat1);
            alLongLat.push_back(long3); alLongLat.push_back(lat3);
            alLongLat.push_back(long0); alLongLat.push_back(lat0);
            alLongLat.push_back(long1); alLongLat.push_back(lat1);
            alLongLat.push_back(long2); alLongLat.push_back(lat2);
            alLongLat.push_back(long3); alLongLat.push_back(lat3);
        }
    }
    vCount = alVertix.size() / 3;
    dataByteCount = vCount * 8 * sizeof(float);
    vdata = new float[vCount * 8];
    int index = 0;
    for (int i = 0; i<vCount; i++)
    {
        vdata[index++] = alVertix[i * 3 + 0];
        vdata[index++] = alVertix[i * 3 + 1];
        vdata[index++] = alVertix[i * 3 + 2];

        vdata[index++] = alVertix[i * 3 + 0] / r;
        vdata[index++] = alVertix[i * 3 + 1] / r;
        vdata[index++] = alVertix[i * 3 + 2] / r;

        vdata[index++] = alLongLat[i * 2 + 0];
        vdata[index++] = alLongLat[i * 2 + 1];
    }
}