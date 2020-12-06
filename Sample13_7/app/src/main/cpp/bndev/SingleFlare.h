
#ifndef SAMPLE13_7_SINGLEFLARE_H
#define SAMPLE13_7_SINGLEFLARE_H
#include <vector>
using namespace std;
class SingleFlare{
public:
    SingleFlare(int idIn, float size, float distance, vector<float> color); //构造函数
    int idIn; //当前光晕元素的纹理序号
    float distance; //当前光晕元素的距离
    float size; //当前光晕元素的原始尺寸值
    float bSize; //当前光晕元素的变换后的尺寸值
    vector<float> color; //当前光晕元素的颜色列表
    float px; //当前光晕元素的x 坐标
    float py; //当前光晕元素的y 坐标
};



#endif //SAMPLE13_7_SINGLEFLARE_H
