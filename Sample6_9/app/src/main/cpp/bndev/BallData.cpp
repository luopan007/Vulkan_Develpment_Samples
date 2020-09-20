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
    const float r=10.0f;
    std::vector<float> alVertix;
    for(float vAngle=90;vAngle>-90;vAngle=vAngle-angleSpan){
        for(float hAngle=360;hAngle>0;hAngle=hAngle-angleSpan){
            double xozLength=r*cos(toRadians(vAngle));
            float x1=(float)(xozLength*cos(toRadians(hAngle)));
            float z1=(float)(xozLength*sin(toRadians(hAngle)));
            float y1=(float)(r*sin(toRadians(vAngle)));
            xozLength=r*cos(toRadians(vAngle-angleSpan));
            float x2=(float)(xozLength*cos(toRadians(hAngle)));
            float z2=(float)(xozLength*sin(toRadians(hAngle)));
            float y2=(float)(r*sin(toRadians(vAngle-angleSpan)));
            xozLength=r*cos(toRadians(vAngle-angleSpan));
            float x3=(float)(xozLength*cos(toRadians(hAngle-angleSpan)));
            float z3=(float)(xozLength*sin(toRadians(hAngle-angleSpan)));
            float y3=(float)(r*sin(toRadians(vAngle-angleSpan)));
            xozLength=r*cos(toRadians(vAngle));
            float x4=(float)(xozLength*cos(toRadians(hAngle-angleSpan)));
            float z4=(float)(xozLength*sin(toRadians(hAngle-angleSpan)));
            float y4=(float)(r*sin(toRadians(vAngle)));
            alVertix.push_back(x1);alVertix.push_back(y1);alVertix.push_back(z1);
            alVertix.push_back(x2);alVertix.push_back(y2);alVertix.push_back(z2);
            alVertix.push_back(x4);alVertix.push_back(y4);alVertix.push_back(z4);
            alVertix.push_back(x4);alVertix.push_back(y4);alVertix.push_back(z4);
            alVertix.push_back(x2);alVertix.push_back(y2);alVertix.push_back(z2);
            alVertix.push_back(x3);alVertix.push_back(y3);alVertix.push_back(z3);
        }}
    vCount = alVertix.size() / 3;
    dataByteCount=vCount*3* sizeof(float);
    vdata=new float[vCount*3];
    int index=0;
    for(int i=0;i<vCount;i++)
    {
        vdata[index++]=alVertix[i*3+0];
        vdata[index++]=alVertix[i*3+1];
        vdata[index++]=alVertix[i*3+2];
    }
}