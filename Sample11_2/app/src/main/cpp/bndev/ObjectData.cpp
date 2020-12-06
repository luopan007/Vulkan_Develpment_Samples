#include "ObjectData.h"
#include <vector>
#include <math.h>
#include <string.h>
float* ObjectData::vdata;
int ObjectData::dataByteCount;
int ObjectData::vCount;
void  ObjectData::genVertexData(){
    std::vector<float> alVertix; //存储顶点位置数据的列表
    std::vector<float> alTexCoor; //存储纹理坐标数据的列表
    float Y_MAX=1.5f; //Y 坐标的最大值
    float Y_MIN=-1.5f; //Y 坐标的最小值
    int FD=6; //长方体每一个侧面中的小矩形个数
    float hw=0.575f; //长方体截面正方形边长的一半
    float yStart=Y_MIN; //起始Y 坐标
    float ySpan=(Y_MAX-Y_MIN)/FD; //Y 坐标的步进
    for(int i=0;i<FD;i++) {//遍历长方体垂直方向分层的每一层
        float x1=-hw; //当前层截面正方形第一个顶点的X 坐标
        float y1=yStart; //当前层截面正方形第一个顶点的Y 坐标
        float z1=hw; //当前层截面正方形第一个顶点的Z 坐标

        float x2=hw;
        float y2=yStart;
        float z2=hw;

        float x3=hw;
        float y3=yStart;
        float z3=-hw;

        float x4=-hw;
        float y4=yStart;
        float z4=-hw;

        float x5=-hw;
        float y5=yStart+ySpan;
        float z5=hw;

        float x6=hw;
        float y6=yStart+ySpan;
        float z6=hw;

        float x7=hw;
        float y7=yStart+ySpan;
        float z7=-hw;

        float x8=-hw;
        float y8=yStart+ySpan;
        float z8=-hw;

        alVertix.push_back(x5);
        alVertix.push_back(y5);
        alVertix.push_back(z5);
        alVertix.push_back(x1);
        alVertix.push_back(y1);
        alVertix.push_back(z1);
        alVertix.push_back(x2);
        alVertix.push_back(y2);
        alVertix.push_back(z2);

        alVertix.push_back(x5);
        alVertix.push_back(y5);
        alVertix.push_back(z5);
        alVertix.push_back(x2);
        alVertix.push_back(y2);
        alVertix.push_back(z2);
        alVertix.push_back(x6);
        alVertix.push_back(y6);
        alVertix.push_back(z6);

        alVertix.push_back(x6);
        alVertix.push_back(y6);
        alVertix.push_back(z6);
        alVertix.push_back(x2);
        alVertix.push_back(y2);
        alVertix.push_back(z2);
        alVertix.push_back(x3);
        alVertix.push_back(y3);
        alVertix.push_back(z3);

        alVertix.push_back(x6);
        alVertix.push_back(y6);
        alVertix.push_back(z6);
        alVertix.push_back(x3);
        alVertix.push_back(y3);
        alVertix.push_back(z3);
        alVertix.push_back(x7);
        alVertix.push_back(y7);
        alVertix.push_back(z7);

        alVertix.push_back(x7);
        alVertix.push_back(y7);
        alVertix.push_back(z7);
        alVertix.push_back(x3);
        alVertix.push_back(y3);
        alVertix.push_back(z3);
        alVertix.push_back(x4);
        alVertix.push_back(y4);
        alVertix.push_back(z4);

        alVertix.push_back(x7);
        alVertix.push_back(y7);
        alVertix.push_back(z7);
        alVertix.push_back(x4);
        alVertix.push_back(y4);
        alVertix.push_back(z4);
        alVertix.push_back(x8);
        alVertix.push_back(y8);
        alVertix.push_back(z8);

        alVertix.push_back(x8);
        alVertix.push_back(y8);
        alVertix.push_back(z8);
        alVertix.push_back(x4);
        alVertix.push_back(y4);
        alVertix.push_back(z4);
        alVertix.push_back(x1);
        alVertix.push_back(y1);
        alVertix.push_back(z1);

        alVertix.push_back(x8);
        alVertix.push_back(y8);
        alVertix.push_back(z8);
        alVertix.push_back(x1);
        alVertix.push_back(y1);
        alVertix.push_back(z1);
        alVertix.push_back(x5);
        alVertix.push_back(y5);
        alVertix.push_back(z5);

        yStart=yStart+ySpan;

        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(0);

        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(0);

        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(0);

        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(0);
        alTexCoor.push_back(0);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(1);
        alTexCoor.push_back(0);
    }
    vCount = FD*4*6;//计算顶点数量
    dataByteCount = vCount * 5 * sizeof(float);//计算数据所占字节数
    vdata = new float[vCount*5];//创建存放数据的数组
    int index = 0;//辅助索引
    for (int i = 0; i < vCount; i++) {//将顶点数据和纹理坐标数据存储到数组中
        vdata[index++] = alVertix[i * 3 + 0];
        vdata[index++] = alVertix[i * 3 + 1];
        vdata[index++] = alVertix[i * 3 + 2];
        vdata[index++] = alTexCoor[i * 2 + 0];
        vdata[index++] = alTexCoor[i * 2 + 1];
    }
}
