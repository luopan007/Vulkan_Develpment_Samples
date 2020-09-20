#ifndef SAMPLE5_1_LANDDATA_H
#define SAMPLE5_1_LANDDATA_H
class LandData
{
public:
    float* vData;
    int vCount;
    LandData(int width, int height, unsigned char* data);
};
#endif
