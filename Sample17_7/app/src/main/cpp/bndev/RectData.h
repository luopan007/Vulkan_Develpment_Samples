#ifndef SAMPLE5_1_RECTDATA_H
#define SAMPLE5_1_RECTDATA_H
#define SKY_BOX_HALF_EDGE 1000
class RectData
{
public:
    static float span;
    static float* vData;
    static int dataByteCount;
    static int vCount;
    static void getVertexData();

};

#endif
