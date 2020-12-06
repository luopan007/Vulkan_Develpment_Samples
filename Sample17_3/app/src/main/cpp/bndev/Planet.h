#include <vector>
using namespace std;
#ifndef SAMPLE7_5_Planet_H
#define SAMPLE7_5_Planet_H
#define instanceCountStrInner(C) #C
class Planet {
public:
    Planet();
    Planet(int id,bool isBigDistance);
    int id;
    vector<float> attributeQiu;//行星的属性
    int paiNumber;
    int groupNumber;
    int oneGroupNumber;
    float rotateAngle;
    float angleSpanAll;
    float distanceDai;
    float linWidthSpan;
    float angleSpan;
    void initInstanceData();//初始化实例数据的方法
    void update();//更新数据的方法
};


#endif //SAMPLE7_5_Planet_H
