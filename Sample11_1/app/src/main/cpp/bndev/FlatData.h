#ifndef VULKANEXBASE_FLATDATA_H
#define VULKANEXBASE_FLATDATA_H
class FlatData
{
public:
    static float* vdata;
    static int dataByteCount;
    static int vCount;
    static void genVertexData();
};
#endif
