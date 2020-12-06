#include "TreeLeaves.h"
#include <math.h>
#define PI 3.1415926535898
float* TreeLeaves::vdata;
int TreeLeaves::dataByteCount;
int TreeLeaves::vCount;
void  TreeLeaves::genVertexData(float width,float height,float absolute_height){
    vCount=6;//设置顶点个数
    dataByteCount=vCount*5* sizeof(float);
    vdata=new float[vCount*5]{
            0,0.3125f*height,0, 1,0,//树叶旋转角度为0  0.6875
            0,-0.6875f*height,0, 1,1,
            width,0.3128f*height,0,0,0,

            width,0.3125f*height,0, 0,0,
            0,-0.6875f*height,0,    1,1,
            width,-0.6875f*height,0,    0,1
    };
}