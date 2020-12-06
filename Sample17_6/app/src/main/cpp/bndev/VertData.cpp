#include "VertData.h"
#include <vector>
#include <math.h>
#include <string.h>
#include "mylog.h"
#include <assert.h>

float* VertData::vdata;
int VertData::dataByteCount;
int VertData::vCount;

int* VertData::indexData;
int VertData::indexDataByteCount;
int VertData::indexCount;

float toRadians(float degree)
{
    return degree*3.1415926535898/180;
}

//自动切分纹理产生纹理数组的方法
float* generateTexCoor(int bw,int bh)
{
    float* result=new float[(bw+1)*(bh+1)*2];
    float sizew=1.0f/bw;//列数
    float sizeh=1.0f/bh;//行数
    int c=0;
    for(int i=0;i<=bh;i++)
    {
        for(int j=0;j<=bw;j++)
        {
            //每行列一个矩形，由两个三角形构成，共六个点，12个纹理坐标
            float s=j*sizew;
            float t=i*sizeh;//得到i行j列小矩形的左上点的纹理坐标值
            result[c++]=s;
            result[c++]=t;//该矩形左上点纹理坐标值
        }
    }
    return result;
}

void  VertData::genVertData()
{
    float startX=-CR*SPAN/2.0f;
    float startZ=-CR*SPAN/2.0f;

    std::vector<float> alVertix;// 存放顶点坐标的ArrayList
    for(int i=0;i<=CR;i++)
    {//垂直方向
        for(int j=0;j<=CR;j++)
        {//水平方向

            float x1=(float)(j*SPAN)+startX;
            float z1=(float)(i*SPAN)+startZ;
            float y1=0;
            alVertix.push_back(x1);alVertix.push_back(y1);alVertix.push_back(z1);
        }
    }

    //获取切分整图的纹理数组
    float* texCoor=generateTexCoor(
            CR, //纹理图切分的列数
            CR  //纹理图切分的行数
    );

    vCount = alVertix.size() / 3;// 顶点的数量为坐标值数量的1/3，因为一个顶点有3个坐标
    dataByteCount=vCount*8* sizeof(float);
    vdata=new float[vCount*8];
    int index=0;
    int texIndex=0;
    for(int i=0;i<vCount;i++)
    {
        //顶点坐标
        vdata[index++]=alVertix[i*3+0];
        vdata[index++]=alVertix[i*3+1];
        vdata[index++]=alVertix[i*3+2];

        vdata[index++]=texCoor[texIndex++];//纹理s坐标
        vdata[index++]=texCoor[texIndex++];//纹理s坐标

        //法向量
        vdata[index++]=0;
        vdata[index++]=1;
        vdata[index++]=0;
    }

    //组织顶点组装索引数据
    indexCount=CR*CR*6;
    indexData=new int[indexCount];
    int tempCount=0;
    for(int i=0;i<CR;i++)
    {
        for(int j=0;j<CR;j++)
        {
            //0---1
            //| / |
            //3---2
            int widthTemp=CR+1;
            int index0=j*widthTemp+i;
            int index1=index0+1;
            int index2=index0+1+widthTemp;
            int index3=index0+widthTemp;

            //0-3-1
            indexData[tempCount*6]=index0;
            indexData[tempCount*6+1]=index3;
            indexData[tempCount*6+2]=index1;

            //1-3-2
            indexData[tempCount*6+3]=index1;
            indexData[tempCount*6+4]=index3;
            indexData[tempCount*6+5]=index2;

            tempCount++;
        }
    }
    indexDataByteCount=indexCount*4;
}