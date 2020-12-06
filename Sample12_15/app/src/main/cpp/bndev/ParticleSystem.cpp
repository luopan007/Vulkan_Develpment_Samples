#include <vector>
#include <math.h>
#include <ParticleForDraw.h>
#include "mylog.h"
#include "ParticleSystem.h"
#include "ParticleDataConstant.cpp"
#include "../util/HelpFunction.h"
#include "../util/CameraUtil.h"
#include <algorithm>
ParticleSystem::ParticleSystem(int count,float  x,float z,int idIn) {
    this->sx=0;
    this->sy=0;
    this->xRange=X_RANGE[idIn];
    this->yRange=Y_RANGE[idIn];
    this->halfSize=RADIS[idIn];
    this->groupCount=GROUP_COUNT[idIn];
    this->lifeSpanStep=LIFE_SPAN_STEP[idIn];
    this->points=initPoints(count);
    this->maxLifeSpan=MAX_LIFE_SPAN[idIn];
    this->vy=VY[idIn];
    this->yAngle=0;
    this->positionX=x;
    this->positionZ=z;
    this->id=idIn;
    this->countIndex=1;
}
std::vector<float> ParticleSystem::initPoints(int zcount){
    for(int i=0;i<zcount;i++){
        float px=(float) (sx+xRange*random(0.0f,1.0f)-xRange/2);//计算粒子位置x坐标
        float py=(float) (sy+yRange*random(0.0f,1.0f)-yRange/2);//计算粒子位置y坐标
        float vx=(sx-px)/150;//计算粒子的x 方向的运动速度
        points.push_back(px);//计算粒子的x 方向的运动速度
        points.push_back(py);//计算粒子的y 方向的运动速度
        points.push_back(vx);//粒子对应的第一个点的x 方向运动速度
        points.push_back(10.0f);//粒子对应的第一个点的当前生命期——10 代表粒子处于未激活状态
    }
    for(int j=0;j<groupCount;j++)//循环遍历第一批粒子
    {
        points[4*j+3]=lifeSpanStep;//设置粒子第1 个点的生命期，不为10 表示粒子处于活跃状态
    }
    vCount = points.size()/4;//计算顶点数量
    dataByteCount = vCount * 4 * sizeof(float);//计算顶点数据所占字节数
    vdata = new float[vCount * 4];//创建顶点数据数组
    return points;
}
void ParticleSystem::genVertexData(){//生成数据的方法
    int index = 0;//辅助索引
    for (int i = 0; i < vCount; i++) {//遍历顶点列表
        vdata[index++] = points[i * 4 + 0];//顶点X 坐标
        vdata[index++] = points[i * 4 + 1];//顶点Y 坐标
        vdata[index++] = points[i * 4 + 2];//顶点X 方向速度
        vdata[index++] = points[i * 4 + 3];//顶点当前生存期
    }
}
void ParticleSystem::update() {//更新粒子状态的方法
    if(countIndex>=(points.size()/groupCount/4))//计数器值超过总粒子数时
    {
        countIndex=0;//从头重新开始计数
    }
    for(int i=0;i<points.size()/4;i++)//循环遍历所有粒子
    {
        if(points[i*4+3]!=10.0f)//若当前为活跃粒子
        {
            points[i*4+3]+=lifeSpanStep;//计算当前生命周期
            if(points[i*4+3]>maxLifeSpan)
            {
                float px=(float) (sx+xRange*random(0.0f,1.0f)-xRange/2);//计算下一轮的x 坐标
                float py=(float) (sy+yRange*random(0.0f,1.0f)-yRange/2);//计算下一轮的y 坐标
                float vx=(sx-px)/150;//计算粒子下一轮x 方向的速度
                points[i*4]=px;//粒子对应的第一个顶点的x 坐标
                points[i*4+1]=py;//粒子对应的第一个顶点的y 坐标
                points[i*4+2]=vx;//粒子对应的x方向速度
                points[i*4+3]=10.0f;//粒子对应的声明周期
            }else
            {
                points[i*4]+=points[i*4+2];//计算粒子对应的第一个顶点的x 坐标
                points[i*4+1]+=vy;//计算粒子对应的第一个顶点的y 坐标
            }
        }
    }
    for(int i=0;i<groupCount;i++)//循环发射一批激活计数器所指定索引的粒子
    {
        if(points[groupCount*countIndex*4+4*i+3]==10.0f)//如果粒子处于未激活状态时
        {
            points[groupCount*countIndex*4+4*i+3]=lifeSpanStep;//激活对应的粒子
        }
    }
    countIndex++;//下次激活粒子的批次索引
    calculateBillboardDirection();//计算粒子系统标志版朝向的方法
}
void ParticleSystem::calculateBillboardDirection() {
    float xspan=positionX-CameraUtil::cx;
    float zspan=positionZ-CameraUtil::cz;
    if(zspan<=0)
    {
        yAngle=-(float)toDegrees(atan(xspan/zspan));
    }
    else
    {
        yAngle=180-(float)toDegrees(atan(xspan/zspan));
    }
}
ParticleSystem::~ParticleSystem(){
    delete [] vdata;
};