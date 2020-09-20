
#ifndef SAMPLE5_1_SKYDATA_H
#define SAMPLE5_1_SKYDATA_H
class SkyData
{
public:
    static float* vdata;
    static int dataByteCount;
    static int vCount;
    static void  genSkyData(int vCountIn);
};
#endif 
