//
// Created by Administrator on 2017\10\25 0025.
//

#ifndef VULKANEXBASE_COLORRECT_H
#define VULKANEXBASE_COLORRECT_H


#include <cstdint>

class ColorRect
{
public:
    static float* vdata;
    static int dataByteCount;
    static int vCount;

    static void  genVertexData();
};

#endif //VULKANEXBASE_COLORRECT_H