#include "Flare.h"
#include "Constant.h"
#include <math.h>
Flare::Flare()
{
    initFlare();//初始化光晕元素列表
}

void Flare::initFlare() {//初始化光晕元素列表的方法
    sFl.push_back((new SingleFlare(1, 5.4f, -1.0f, vector<float> {1.0f, 1.0f, 1.0f, 1.0f})));
    sFl.push_back((new SingleFlare(1, 0.4f, -0.8f, vector<float>  {0.7f, 0.5f, 0.0f, 0.02f})));
    sFl.push_back((new SingleFlare(1, 0.04f, -0.7f, vector<float>  {1.0f, 0.0f, 0.0f, 0.07f})));
    sFl.push_back((new SingleFlare(0, 0.4f, -0.5f, vector<float>  {1.0f, 1.0f, 0.0f, 0.05f})));
    sFl.push_back((new SingleFlare(2, 1.22f, -0.4f, vector<float>  {1.0f, 1.0f, 0.0f, 0.05f})));
    sFl.push_back((new SingleFlare(0, 0.4f, -0.3f, vector<float>  {1.0f, 0.5f, 0.0f, 1.0f})));
    sFl.push_back((new SingleFlare(1, 0.4f, -0.1f, vector<float> {1.0f, 1.0f, 0.5f, 0.05f})));
    sFl.push_back((new SingleFlare(0, 0.4f, 0.2f, vector<float>  {1.0f, 0.0f, 0.0f, 1.0f})));
    sFl.push_back((new SingleFlare(1, 0.8f, 0.3f, vector<float>  {1.0f, 1.0f, 0.6f, 1.0f})));
    sFl.push_back((new SingleFlare(0, 0.6f, 0.4f, vector<float>  {1.0f, 0.7f, 0.0f, 0.03f})));
    sFl.push_back((new SingleFlare(2, 0.6f, 0.7f, vector<float>  {1.0f, 0.5f, 0.0f, 0.02f})));
    sFl.push_back((new SingleFlare(2, 1.28f, 1.0f, vector<float>  {1.0f, 0.7f, 0.0f, 0.02f})));
    sFl.push_back((new SingleFlare(2, 3.20f, 1.3f, vector<float>  {1.0f, 0.0f, 0.0f, 0.3f})));
}
void Flare::update(float lx, float ly) {//更新光晕元素位置和尺寸的方法
    float currDis = (float)sqrt(lx*lx + ly*ly);//计算光源在屏幕上的投影位置距屏幕中心点的距离
    float currScale = Constant::SCALE_MIN + (Constant::SCALE_MAX - Constant::SCALE_MIN)*(1 - currDis / Constant::DIS_MAX);//计算尺寸缩放系数
    for (auto  ss : sFl)//遍历各个光晕元素
    {
        ss->px = -ss->distance*lx;//更新x 坐标
        ss->py = ss->distance*ly;//更新y 坐标
        ss->bSize = ss->size*currScale;//更新尺寸
    }
}