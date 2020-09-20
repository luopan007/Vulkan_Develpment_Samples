#include "CubeData.h"
#include <vector>
#include <math.h>
#include <string.h>
float* CubeData::vdata;
int CubeData::dataByteCount;
int CubeData::vCount;
void  CubeData::genBallData()
{
    float UNIT_SIZE=1;
    float vertices[]=
            {
                    
                    0,0,UNIT_SIZE,
                    UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    -UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    0,0,UNIT_SIZE,
                    -UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    -UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
                    0,0,UNIT_SIZE,
                    -UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
                    UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
                    0,0,UNIT_SIZE,
                    UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
                    UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    
                    0,0,-UNIT_SIZE,
                    UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    0,0,-UNIT_SIZE,
                    UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    -UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    0,0,-UNIT_SIZE,
                    -UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    -UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    0,0,-UNIT_SIZE,
                    -UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    
                    -UNIT_SIZE,0,0,
                    -UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    -UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    -UNIT_SIZE,0,0,
                    -UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    -UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    -UNIT_SIZE,0,0,
                    -UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    -UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
                    -UNIT_SIZE,0,0,
                    -UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
                    -UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    
                    UNIT_SIZE,0,0,
                    UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
                    UNIT_SIZE,0,0,
                    UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
                    UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    UNIT_SIZE,0,0,
                    UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    UNIT_SIZE,0,0,
                    UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    
                    0,UNIT_SIZE,0,
                    UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    0,UNIT_SIZE,0,
                    UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    -UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    0,UNIT_SIZE,0,
                    -UNIT_SIZE,UNIT_SIZE,-UNIT_SIZE,
                    -UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    0,UNIT_SIZE,0,
                    -UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    UNIT_SIZE,UNIT_SIZE,UNIT_SIZE,
                    
                    0,-UNIT_SIZE,0,
                    UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
                    -UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
                    0,-UNIT_SIZE,0,
                    -UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
                    -UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    0,-UNIT_SIZE,0,
                    -UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    0,-UNIT_SIZE,0,
                    UNIT_SIZE,-UNIT_SIZE,-UNIT_SIZE,
                    UNIT_SIZE,-UNIT_SIZE,UNIT_SIZE,
            };
    float colors[]={
            
            1,1,1,
            1,0,0,
            1,0,0,
            1,1,1,
            1,0,0,
            1,0,0,
            1,1,1,
            1,0,0,
            1,0,0,
            1,1,1,
            1,0,0,
            1,0,0,
            
            1,1,1,
            0,0,1,
            0,0,1,
            1,1,1,
            0,0,1,
            0,0,1,
            1,1,1,
            0,0,1,
            0,0,1,
            1,1,1,
            0,0,1,
            0,0,1,
            
            1,1,1,
            1,0,1,
            1,0,1,
            1,1,1,
            1,0,1,
            1,0,1,
            1,1,1,
            1,0,1,
            1,0,1,
            1,1,1,
            1,0,1,
            1,0,1,
            
            1,1,1,
            1,1,0,
            1,1,0,
            1,1,1,
            1,1,0,
            1,1,0,
            1,1,1,
            1,1,0,
            1,1,0,
            1,1,1,
            1,1,0,
            1,1,0,
            
            1,1,1,
            0,1,0,
            0,1,0,
            1,1,1,
            0,1,0,
            0,1,0,
            1,1,1,
            0,1,0,
            0,1,0,
            1,1,1,
            0,1,0,
            0,1,0,
            
            1,1,1,
            0,1,1,
            0,1,1,
            1,1,1,
            0,1,1,
            0,1,1,
            1,1,1,
            0,1,1,
            0,1,1,
            1,1,1,
            0,1,1,
            0,1,1,
    };
    vCount = (sizeof(vertices)/sizeof(vertices[0]))/ 3;
    
    dataByteCount=(sizeof(vertices)/sizeof(vertices[0]))*2* sizeof(float);
    vdata=new float[(sizeof(vertices)/sizeof(vertices[0]))*2];
    int index=0;
    for(int i=0;i<vCount;i++)
    {
        vdata[index++]=vertices[i*3+0];
        vdata[index++]=vertices[i*3+1];
        vdata[index++]=vertices[i*3+2];
        vdata[index++]=colors[i*3+0];
        vdata[index++]=colors[i*3+1];
        vdata[index++]=colors[i*3+2];
    }
}