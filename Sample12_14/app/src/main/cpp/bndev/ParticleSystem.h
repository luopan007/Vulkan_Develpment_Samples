#ifndef SAMPLE_SHI_PARTICLESYSTEM_H
#define SAMPLE_SHI_PARTICLESYSTEM_H
#include <ObjObject.h>
#include <vector>
class ParticleSystem{
public:
    float* vdata; //顶点数据数组首地址指针
    int dataByteCount; //顶点数据所占总字节数
    int vCount; //顶点数量
    void genVertexData(); //生成顶点数据的方法
    void update(); //更新粒子位置的方法
    void calculateBillboardDirection(); //计算标志版朝向的方法
    std::vector<float> initPoints(int zcount); //初始化顶点位置的方法
    std::vector<float> points; //存放顶点数据的列表
    std::vector<float> texCoor; //存放纹理坐标数据的列表
    float positionX; //粒子系统位置x 坐标
    float positionZ; //粒子系统位置z 坐标
    float sx; //粒子系统生成粒子的起始x 坐标
    float sy; //粒子系统生成粒子的起始y 坐标
    float xRange; //粒子系统生成粒子的起始z 跨度
    float yRange; //粒子系统生成粒子的起始y 跨度
    float halfSize; //每一个粒子的半径
    float groupCount; //每帧发射的组数
    float lifeSpanStep; //每个粒子的生命期步进
    float maxLifeSpan; //每个粒子的最大生命期
    float vy; //粒子Y 方向速度
    float yAngle; //粒子Y 轴旋转角度（设置朝向时使用）
    int id; //粒子系统当前的ID
    int countIndex; //粒子系统的激活批次索引
    ParticleSystem(int count,float x,float z,int idIn); //带有参数的构造函数
    ParticleSystem(); //无参数的构造函数
    ~ParticleSystem(); //析构函数
};



#endif //SAMPLE_SHI_PARTICLESYSTEM_H
