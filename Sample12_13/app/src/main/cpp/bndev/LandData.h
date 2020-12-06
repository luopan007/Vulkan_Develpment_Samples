#ifndef VULKANEXBASE_LAND_H
#define VULKANEXBASE_LAND_H
class LandData
{
public:
    float* vData;//生成的顶点mesh数据，包含顶点坐标、纹理坐标
    int vCount;//顶点数量
    LandData(int width, int height, unsigned char* data);
};


#endif
