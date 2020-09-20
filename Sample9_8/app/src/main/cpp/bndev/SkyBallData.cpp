#include <vector>
#include "HelpFunction.h"
#include "SkyBallData.h"
#include "mylog.h"
#include <math.h>
#include <Constant.h>
float* SkyBallData::vdata;
int SkyBallData::dataByteCount;
int SkyBallData::vCount;
void SkyBallData::genVertexData() { //生成天空穹半球顶点数据的方法
    float radius=Constant::SKY_R; //天空穹的半径
    float ANGLE_SPAN=18; //切分间隔角度
    float angleV=90; //纵向上的起始度数
    std::vector<float> texCoorArray= generateSkyTexCoor((int)(360/ANGLE_SPAN),
                                                        (int)(angleV/ANGLE_SPAN)); //生成天空穹纹理坐标数据
    int tc=0; //纹理坐标数组辅助索引
    int ts=texCoorArray.size(); //纹理坐标数据数量
    std::vector<float> alVertix; //存放结果顶点数据的列表
    std::vector<float> alTexCoor; //存放结果纹理坐标数据的列表
    for(float vAngle=angleV;vAngle>0;vAngle=vAngle-ANGLE_SPAN) { //垂直方向分割
        for(float hAngle=360;hAngle>0;hAngle=hAngle-ANGLE_SPAN) { //水平方向分割
            double xozLength=radius*cos(toRadians(vAngle)); //获取xz 平面参数
            float x1=(float)(xozLength*cos(toRadians(hAngle))); //获取X 坐标
            float z1=(float)(xozLength*sin(toRadians(hAngle))); //获取Z 坐标
            float y1=(float)(radius*sin(toRadians(vAngle))); //获取Y 坐标
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
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
        }
    }
    vCount = alVertix.size()/3;
    dataByteCount = vCount * 5 * sizeof(float);
    vdata = new float[vCount*5];
    int index = 0; //辅助索引
    for (int i = 0; i < vCount; i++) { //遍历每个顶点
        vdata[index++] = alVertix[i * 3 + 0]; //保存顶点X 坐标
        vdata[index++] = alVertix[i * 3 + 1]; //保存顶点Y 坐标
        vdata[index++] = alVertix[i * 3 + 2]; //保存顶点Z 坐标
        vdata[index++] = alTexCoor[i * 2 + 0]; //保存纹理S 坐标
        vdata[index++] = alTexCoor[i * 2 + 1]; //保存纹理T 坐标
    }
}