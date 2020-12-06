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
    this->sx=0;//初始化粒子系统生成粒子的起始x 坐标
    this->sy=0;//初始化粒子系统生成粒子的起始y 坐标
    this->xRange=X_RANGE[idIn];//初始化粒子系统生成粒子的x 跨度
    this->yRange=Y_RANGE[idIn];//初始化粒子系统生成粒子的y 跨度
    this->halfSize=RADIS[idIn];//初始化粒子半径
    this->groupCount=GROUP_COUNT[idIn];//初始化组数
    this->lifeSpanStep=LIFE_SPAN_STEP[idIn];//初始化声明周期
    this->points=initPoints(count);//初始化当前顶点
    this->maxLifeSpan=MAX_LIFE_SPAN[idIn];//初始化最大生命周期
    this->vy=VY[idIn];//初始化当前Y方向的速度
    this->yAngle=0;//初始化Y方向速度
    this->positionX=x;//初始化当前x位置
    this->positionZ=z;//初始化当前y位置
    this->id=idIn;//初始化当前id
    this->countIndex=1;//初始化辅助索引
}
std::vector<float> ParticleSystem::initPoints(int zcount){
    for(int i=0;i<zcount;i++){//循环遍历每个粒子
        float px=(float) (sx+xRange*random(0.0f,1.0f)-xRange/2);//计算粒子位置x坐标
        float py=(float) (sy+yRange*random(0.0f,1.0f)-yRange/2);//计算粒子位置y坐标
        float vx=(sx-px)/150;//计算粒子的x 方向的运动速度
        points.push_back(px-halfSize/2);//粒子对应的第一个点的x 坐标
        points.push_back(py+halfSize/2);//粒子对应的第一个点的y 坐标
        points.push_back(vx);//粒子对应的第一个点的x 方向运动速度
        points.push_back(10.0f);//粒子对应的第一个点的当前生命期——10 代表粒子处于未激活状态
        //其余6个顶点的坐标
        points.push_back(px-halfSize/2);
        points.push_back(py-halfSize/2);
        points.push_back(vx);
        points.push_back(10.0f);

        points.push_back(px+halfSize/2);
        points.push_back(py+halfSize/2);
        points.push_back(vx);
        points.push_back(10.0f);

        points.push_back(px+halfSize/2);
        points.push_back(py+halfSize/2);
        points.push_back(vx);
        points.push_back(10.0f);

        points.push_back(px-halfSize/2);
        points.push_back(py-halfSize/2);
        points.push_back(vx);
        points.push_back(10.0f);

        points.push_back(px+halfSize/2);
        points.push_back(py-halfSize/2);
        points.push_back(vx);
        points.push_back(10.0f);
    }
    for(int j=0;j<groupCount;j++)//循环遍历第一批粒子
    {
        points[4*j*6+3]=lifeSpanStep;//设置粒子第1 个点的生命期，不为10 表示粒子处于活跃状态
        points[4*j*6+7]=lifeSpanStep;//设置粒子第2 个点的生命期，不为10 表示粒子处于活跃状态
        points[4*j*6+11]=lifeSpanStep;//设置粒子第3 个点的生命期，不为10 表示粒子处于活跃状态
        points[4*j*6+15]=lifeSpanStep;//设置粒子第4 个点的生命期，不为10 表示粒子处于活跃状态
        points[4*j*6+19]=lifeSpanStep;//设置粒子第5 个点的生命期，不为10 表示粒子处于活跃状态
        points[4*j*6+23]=lifeSpanStep;//设置粒子第6 个点的生命期，不为10 表示粒子处于活跃状态
    }
    vCount = points.size()/4;//计算顶点数量
    dataByteCount = vCount * 6 * sizeof(float);//计算顶点数据所占字节数
    vdata = new float[vCount * 6];//创建顶点数据数组
    for(int i=0;i<vCount/6;i++)//循环遍历每个粒子，设置对应纹理坐标
    {
        texCoor.push_back(0);
        texCoor.push_back(0);

        texCoor.push_back(0);
        texCoor.push_back(1);

        texCoor.push_back(1);
        texCoor.push_back(0);

        texCoor.push_back(1);
        texCoor.push_back(0);

        texCoor.push_back(0);
        texCoor.push_back(1);

        texCoor.push_back(1);
        texCoor.push_back(1);
    }
    return points;
}
void ParticleSystem::genVertexData(){//生成数据的方法
    int index = 0;//辅助索引
    for (int i = 0; i < vCount; i++) {//遍历顶点列表
        vdata[index++] = points[i * 4 + 0];//顶点X 坐标
        vdata[index++] = points[i * 4 + 1];//顶点Y 坐标
        vdata[index++] = points[i * 4 + 2];//顶点X 方向速度
        vdata[index++] = points[i * 4 + 3];//顶点当前生存期
        vdata[index++] = texCoor[i * 2 + 0];//顶点纹理S 坐标
        vdata[index++] = texCoor[i * 2 + 1];//顶点纹理Z 坐标
    }
}
void ParticleSystem::update() {//更新粒子状态的方法
    if(countIndex>=(points.size()/groupCount/4/6))//计数器值超过总粒子数时
    {
        countIndex=0;//从头重新开始计数
    }
    for(int i=0;i<points.size()/4/6;i++)//循环遍历所有粒子
    {
        if(points[i*4*6+3]!=10.0f)//若当前为活跃粒子
        {
            points[i*4*6+3]+=lifeSpanStep;//计算当前生命周期
            points[i*4*6+7]+=lifeSpanStep;
            points[i*4*6+11]+=lifeSpanStep;
            points[i*4*6+15]+=lifeSpanStep;
            points[i*4*6+19]+=lifeSpanStep;
            points[i*4*6+23]+=lifeSpanStep;
            if(points[i*4*6+3]>maxLifeSpan)//当前生命期大于最大生命期时
            {
                float px=(float) (sx+xRange*random(0.0f,1.0f)-xRange/2);//计算下一轮的x 坐标
                float py=(float) (sy+yRange*random(0.0f,1.0f)-yRange/2);//计算下一轮的y 坐标
                float vx=(sx-px)/150.0f;//计算粒子下一轮x 方向的速度
                points[i*4*6]=px-halfSize/2;//粒子对应的第一个顶点的x 坐标
                points[i*4*6+1]=py+halfSize/2;//粒子对应的第一个顶点的y 坐标
                points[i*4*6+2]=vx;//粒子对应的第一个顶点的x 方向的运动速度
                points[i*4*6+3]=10.0f;//粒子对应的第一个顶点的当前生命期
                points[i*4*6+4]=px-halfSize/2;
                points[i*4*6+5]=py-halfSize/2;
                points[i*4*6+6]=vx;
                points[i*4*6+7]=10.0f;
                points[i*4*6+8]=px+halfSize/2;
                points[i*4*6+9]=py+halfSize/2;
                points[i*4*6+10]=vx;
                points[i*4*6+11]=10.0f;
                points[i*4*6+12]=px+halfSize/2;
                points[i*4*6+13]=py+halfSize/2;
                points[i*4*6+14]=vx;
                points[i*4*6+15]=10.0f;
                points[i*4*6+16]=px-halfSize/2;
                points[i*4*6+17]=py-halfSize/2;
                points[i*4*6+18]=vx;
                points[i*4*6+19]=10.0f;
                points[i*4*6+20]=px+halfSize/2;
                points[i*4*6+21]=py-halfSize/2;
                points[i*4*6+22]=vx;
                points[i*4*6+23]=10.0f;
            }else{
                points[i*4*6]+=points[i*4*6+2];//计算粒子对应的第一个顶点的x 坐标
                points[i*4*6+1]+=vy;//计算粒子对应的第一个顶点的y 坐标
                points[i*4*6+4]+=points[i*4*6+6];
                points[i*4*6+5]+=vy;
                points[i*4*6+8]+=points[i*4*6+10];
                points[i*4*6+9]+=vy;
                points[i*4*6+12]+=points[i*4*6+14];
                points[i*4*6+13]+=vy;
                points[i*4*6+16]+=points[i*4*6+18];
                points[i*4*6+17]+=vy;
                points[i*4*6+20]+=points[i*4*6+22];
                points[i*4*6+21]+=vy;
            }
        }
    }
    for(int i=0;i<groupCount;i++)//循环发射一批激活计数器所指定索引的粒子
    {
        if(points[groupCount*countIndex*4*6+4*i*6+3]==10.0f)//如果粒子处于未激活状态时
        {
            points[groupCount*countIndex*4*6+4*i*6+3]=lifeSpanStep;//激活对应的粒子
            points[groupCount*countIndex*4*6+4*i*6+7]=lifeSpanStep;//激活对应的粒子
            points[groupCount*countIndex*4*6+4*i*6+11]=lifeSpanStep;//激活对应的粒子
            points[groupCount*countIndex*4*6+4*i*6+15]=lifeSpanStep;//激活对应的粒子
            points[groupCount*countIndex*4*6+4*i*6+19]=lifeSpanStep;//激活对应的粒子
            points[groupCount*countIndex*4*6+4*i*6+23]=lifeSpanStep;//激活对应的粒子
        }
    }
    countIndex++;//下次激活粒子的批次索引
    calculateBillboardDirection();//计算粒子系统标志版朝向的方法
}
void ParticleSystem::calculateBillboardDirection() {//计算标志版朝向的方法
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