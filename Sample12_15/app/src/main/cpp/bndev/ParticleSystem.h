#ifndef SAMPLE_SHI_PARTICLESYSTEM_H
#define SAMPLE_SHI_PARTICLESYSTEM_H
#include <ObjObject.h>
#include <vector>
class ParticleSystem{
public:
    float* vdata;
    int dataByteCount;
    int vCount;
    void genVertexData();
    void update();
    void calculateBillboardDirection();
    std::vector<float> initPoints(int zcount);
    std::vector<float> points;
    float positionX;
    float positionZ;
    float sx;
    float sy;
    float xRange;
    float yRange;
    float halfSize;
    float groupCount;
    float lifeSpanStep;
    float maxLifeSpan;
    float vy;
    float yAngle;
    int id;
    int countIndex;
    ParticleSystem(int count,float  x,float z,int idIn);
    ParticleSystem();
    ~ParticleSystem();
};


#endif //SAMPLE_SHI_PARTICLESYSTEM_H
