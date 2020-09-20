#ifndef VULKANEXBASE_BELTDATA_H
#define VULKANEXBASE_BELTDATA_H

#include <cstdint>

class CircleData
{
public:
    static float* vdata;
    static int dataByteCount;
    static int vCount;

    static uint16_t* idata; //索引数据指针
    static int indexByteCount; //索引数据所占总字节数
    static uint32_t iCount; //索引数量
    static double toRadians(double d);
    static void  genVertexData();
};


#endif
