#ifndef VULKANEXBASE_BELTDATA_H
#define VULKANEXBASE_BELTDATA_H
#include <cstdint>
class CircleData
{
public:
    static float* vdata;
    static int dataByteCount;
    static int vCount;
    static uint16_t* idata; 
    static int indexByteCount; 
    static uint32_t iCount; 
    static double toRadians(double d);
    static void  genVertexData();
};
#endif
