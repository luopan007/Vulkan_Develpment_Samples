#include <math.h>
#include "SkyCloudData.h"
#include <iostream>
#include <vector>
#include "FileUtil.h"
float* SkyCloudData::vdata;
int SkyCloudData::dataByteCount;
int SkyCloudData::vCount;
float SkyCloudData::radius=300.0f;
float toRadians(float degree){
    return degree*3.1415926535898/180;
}
void SkyCloudData::initData()
{
    float ANGLE_SPAN=18.0f;
    float angleV=90;
    vector<float> alVertix;
    for(float vAngle=angleV;vAngle>0;vAngle=vAngle-ANGLE_SPAN)
    {
        for(float hAngle=360;hAngle>0;hAngle=hAngle-ANGLE_SPAN)
        {
            double xozLength=radius*cos(toRadians(vAngle));
            float x1=(float)(xozLength*cos(toRadians(hAngle)));
            float z1=(float)(xozLength*sin(toRadians(hAngle)));
            float y1=(float)(radius*sin(toRadians(vAngle)));
            xozLength=radius*cos(toRadians(vAngle-ANGLE_SPAN));
            float x2=(float)(xozLength*cos(toRadians(hAngle)));
            float z2=(float)(xozLength*sin(toRadians(hAngle)));
            float y2=(float)(radius*sin(toRadians(vAngle-ANGLE_SPAN)));
            xozLength=radius*cos(toRadians(vAngle-ANGLE_SPAN));
            float x3=(float)(xozLength*cos(toRadians(hAngle-ANGLE_SPAN)));
            float z3=(float)(xozLength*sin(toRadians(hAngle-ANGLE_SPAN)));
            float y3=(float)(radius*sin(toRadians(vAngle-ANGLE_SPAN)));
            xozLength=radius*cos(toRadians(vAngle));
            float x4=(float)(xozLength*cos(toRadians(hAngle-ANGLE_SPAN)));
            float z4=(float)(xozLength*sin(toRadians(hAngle-ANGLE_SPAN)));
            float y4=(float)(radius*sin(toRadians(vAngle)));
            alVertix.push_back(x1);alVertix.push_back(y1);alVertix.push_back(z1);
            alVertix.push_back(x4);alVertix.push_back(y4);alVertix.push_back(z4);
            alVertix.push_back(x2);alVertix.push_back(y2);alVertix.push_back(z2);
            alVertix.push_back(x2);alVertix.push_back(y2);alVertix.push_back(z2);
            alVertix.push_back(x4);alVertix.push_back(y4);alVertix.push_back(z4);
            alVertix.push_back(x3);alVertix.push_back(y3);alVertix.push_back(z3);
        }
    }
    vCount=alVertix.size()/3;
    float* vertices=new float[vCount*3];
    for(int i=0;i<alVertix.size();i++)
    {
        vertices[i]=alVertix[i];
    }
    float* textureCoors=generateTexCoor
            (
                    (int)(360/ANGLE_SPAN), 
                    (int)(angleV/ANGLE_SPAN)  
            );
    dataByteCount=vCount*5*sizeof(float);
    vdata=new float[vCount*5];
    int indexTemp=0;
    for(int i=0;i<vCount;i++)
    {
        vdata[indexTemp++]=vertices[i*3+0];
        vdata[indexTemp++]=vertices[i*3+1];
        vdata[indexTemp++]=vertices[i*3+2];
        vdata[indexTemp++]=textureCoors[i*2+0];
        vdata[indexTemp++]=textureCoors[i*2+1];
    }
}
float * SkyCloudData::generateTexCoor(int bw,int bh)
{
    float* result=new float[bw*bh*6*2];
        float sizew=1.0f/bw;
        float sizeh=1.0f/bh;
        int c=0;
        for(int i=0;i<bh;i++)
        {
            for(int j=0;j<bw;j++)
            {
                float s=j*sizew;
                float t=i*sizeh;
                result[c++]=s;
                result[c++]=t;
                result[c++]=s+sizew;
                result[c++]=t;
                result[c++]=s;
                result[c++]=t+sizeh;
                result[c++]=s;
                result[c++]=t+sizeh;
                result[c++]=s+sizew;
                result[c++]=t;
                result[c++]=s+sizew;
                result[c++]=t+sizeh;
            }
        }
    return result;
}