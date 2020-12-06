#ifndef VULKANEXBASE_LAND_H
#define VULKANEXBASE_LAND_H
class LandData
{
public:
    float* vData;
    int vCount;
    LandData(int width, int height, unsigned char* data);
};
#endif
