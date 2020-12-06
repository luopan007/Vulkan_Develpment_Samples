#include <vector>
#include "HelpFunction.h"
#include "SkyBallData.h"
#include "mylog.h"
#include <math.h>
#include <Constant.h>

float* SkyBallData::vdata;
int SkyBallData::dataByteCount;
int SkyBallData::vCount;

void  SkyBallData::genVertexData() {
    float radius=Constant::Sky_R;
    float ANGLE_SPAN=18;//切分间隔
    float angleV=90;//纵向上的起始度数
    std::vector<float> texCoorArray= generateSkyTexCoor
                            (
                                    (int)(360/ANGLE_SPAN), //纹理图切分的列数
                                    (int)(angleV/ANGLE_SPAN)  //纹理图切分的行数
                            );
    int tc=0;//纹理数组计数器
    int ts=texCoorArray.size();
    std::vector<float> alVertix;
    std::vector<float> alTexCoor;
    for(float vAngle=angleV;vAngle>0;vAngle=vAngle-ANGLE_SPAN)//垂直方向angleSpan度一份
    {
        for(float hAngle=360;hAngle>0;hAngle=hAngle-ANGLE_SPAN)//水平方向angleSpan度一份
        {
            //纵向横向各到一个角度后计算对应的此点在球面上的四边形顶点坐标
            //并构建两个组成四边形的三角形
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

            //构建第一三角形
            alVertix.push_back(x1);alVertix.push_back(y1);alVertix.push_back(z1);
            alVertix.push_back(x4);alVertix.push_back(y4);alVertix.push_back(z4);
            alVertix.push_back(x2);alVertix.push_back(y2);alVertix.push_back(z2);

            //构建第二三角形

            alVertix.push_back(x2);alVertix.push_back(y2);alVertix.push_back(z2);
            alVertix.push_back(x4);alVertix.push_back(y4);alVertix.push_back(z4);
            alVertix.push_back(x3);alVertix.push_back(y3);alVertix.push_back(z3);

            //第一三角形3个顶点的6个纹理坐标
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            alTexCoor.push_back(texCoorArray[tc++%ts]);
            //第二三角形3个顶点的6个纹理坐标
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
    int index = 0;
    for (int i = 0; i < vCount; i++) {
        vdata[index++] = alVertix[i * 3 + 0];
        vdata[index++] = alVertix[i * 3 + 1];
        vdata[index++] = alVertix[i * 3 + 2];
        vdata[index++] = alTexCoor[i * 2 + 0];
        vdata[index++] = alTexCoor[i * 2 + 1];
    }


}