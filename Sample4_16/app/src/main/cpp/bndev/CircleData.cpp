#include <CircleData.h>
#include <vector>
#include <math.h>
#include <string.h>
const double PI=3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170680;
float* CircleData::vdata;
int CircleData::dataByteCount;
int CircleData::vCount;
uint16_t* CircleData::idata;
int CircleData::indexByteCount;
uint32_t CircleData::iCount;
double CircleData::toRadians(double d)
{
    double e=d*PI/180;
    return e;
}
void CircleData::genVertexData(){
    int n = 10; 
    vCount=n+1; 
    dataByteCount=vCount*6* sizeof(float); 
    vdata=new float[vCount*6]; 
    float angdegSpan=360.0f/n; 
    int count=0; 
    vdata[count++] = 0; vdata[count++] = 0;vdata[count++] = 0; 
    vdata[count++] = 1;vdata[count++] = 1; vdata[count++] = 1; 
    for(float angdeg=0; ceil(angdeg)<=360; angdeg+=angdegSpan){ 
        double angrad=toRadians(angdeg); 
        vdata[count++]=(float) (-30*sin(angrad)); 
        vdata[count++]=(float) (30*cos(angrad)); 
        vdata[count++]=0; 
        vdata[count++] = 0;vdata[count++] = 1;vdata[count++] = 0; 
    }
    iCount=12; 
    indexByteCount = iCount * sizeof(uint16_t); 
    idata=new uint16_t[iCount]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1}; 
}