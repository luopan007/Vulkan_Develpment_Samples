#ifndef VULKANEXBASE_BALLDATA_H
#define VULKANEXBASE_BALLDATA_H
#define CR 64
#define WIDTH_SPAN 2.0f*63
class VertData
{
public:
    static float* vdata;
    static int dataByteCount;
    static int vCount;
    static int* indexData;
    static int indexDataByteCount;
    static int indexCount;
    static void  genVertData();
};
#endif
