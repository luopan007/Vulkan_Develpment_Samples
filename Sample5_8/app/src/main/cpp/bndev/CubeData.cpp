#include "CubeData.h"
#include <vector>
#include <math.h>
#include <string.h>

float* CubeData::vdata;
int CubeData::dataByteCount;
int CubeData::vCount;
void  CubeData::genData()
{
    const float rect=1.0f;//立方体的边长
    std::vector<float> alVertix;//存放顶点坐标的vector
    alVertix.push_back(rect);alVertix.push_back(rect);alVertix.push_back(rect);
    alVertix.push_back(-rect);alVertix.push_back(rect);alVertix.push_back(rect);
    alVertix.push_back(-rect);alVertix.push_back(- rect);alVertix.push_back(rect);

    alVertix.push_back(rect);alVertix.push_back(rect);alVertix.push_back(rect);
    alVertix.push_back(-rect);alVertix.push_back(-rect);alVertix.push_back(rect);
    alVertix.push_back(rect);alVertix.push_back(-rect);alVertix.push_back(rect);
    
    alVertix.push_back(rect);alVertix.push_back(rect);alVertix.push_back(-rect);
    alVertix.push_back(-rect);alVertix.push_back(rect);alVertix.push_back(-rect);
    alVertix.push_back(-rect);alVertix.push_back(- rect);alVertix.push_back(-rect);

    alVertix.push_back(rect);alVertix.push_back(rect);alVertix.push_back(-rect);
    alVertix.push_back(-rect);alVertix.push_back(-rect);alVertix.push_back(-rect);
    alVertix.push_back(rect);alVertix.push_back(-rect);alVertix.push_back(-rect);
    
    alVertix.push_back(-rect);alVertix.push_back(-rect);alVertix.push_back(-rect);
    alVertix.push_back(-rect);alVertix.push_back(rect);alVertix.push_back(-rect);
    alVertix.push_back(-rect);alVertix.push_back(rect);alVertix.push_back(rect);

    alVertix.push_back(-rect);alVertix.push_back(-rect);alVertix.push_back(rect);
    alVertix.push_back(-rect);alVertix.push_back(-rect);alVertix.push_back(-rect);
    alVertix.push_back(-rect);alVertix.push_back(rect);alVertix.push_back(rect);
    
    alVertix.push_back(rect);alVertix.push_back(-rect);alVertix.push_back(-rect);
    alVertix.push_back(rect);alVertix.push_back(rect);alVertix.push_back(-rect);
    alVertix.push_back(rect);alVertix.push_back(rect);alVertix.push_back(rect);

    alVertix.push_back(rect);alVertix.push_back(-rect);alVertix.push_back(rect);
    alVertix.push_back(rect);alVertix.push_back(-rect);alVertix.push_back(-rect);
    alVertix.push_back(rect);alVertix.push_back(rect);alVertix.push_back(rect);
    
    alVertix.push_back(-rect);alVertix.push_back(rect);alVertix.push_back(-rect);
    alVertix.push_back(rect);alVertix.push_back(rect);alVertix.push_back(-rect);
    alVertix.push_back(rect);alVertix.push_back(rect);alVertix.push_back(rect);

    alVertix.push_back(-rect);alVertix.push_back(rect);alVertix.push_back(rect);
    alVertix.push_back(-rect);alVertix.push_back(rect);alVertix.push_back(-rect);
    alVertix.push_back(rect);alVertix.push_back(rect);alVertix.push_back(rect);
    
    alVertix.push_back(-rect);alVertix.push_back(-rect);alVertix.push_back(-rect);
    alVertix.push_back(rect);alVertix.push_back(-rect);alVertix.push_back(-rect);
    alVertix.push_back(rect);alVertix.push_back(-rect);alVertix.push_back(rect);

    alVertix.push_back(-rect);alVertix.push_back(-rect);alVertix.push_back(rect);
    alVertix.push_back(-rect);alVertix.push_back(-rect);alVertix.push_back(-rect);
    alVertix.push_back(rect);alVertix.push_back(-rect);alVertix.push_back(rect);

    vCount = alVertix.size() / 3;//顶点的数量为坐标值数量的1/3，因为一个顶点有3 个坐标
    dataByteCount=alVertix.size()*2* sizeof(float);//顶点坐标和法向量数据的总字节数
    vdata=new float[alVertix.size()*2];//存放顶点坐标和法向量数据的数组
    int index=0;//辅助数组索引
    for(int i=0;i<vCount;i++)
    {
        vdata[index++]=alVertix[i*3+0]; //保存顶点坐标X 分量
        vdata[index++]=alVertix[i*3+1]; //保存顶点坐标Y 分量
        vdata[index++]=alVertix[i*3+2]; //保存顶点坐标Z 分量
        vdata[index++]=alVertix[i*3+0]; //保存顶点法向量X 分量
        vdata[index++]=alVertix[i*3+1]; //保存顶点法向量Y 分量
        vdata[index++]=alVertix[i*3+2]; //保存顶点法向量Z 分量
    }
}