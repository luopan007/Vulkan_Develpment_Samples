#ifndef SAMPLE7_1_RECTDATA_H
#define SAMPLE7_1_RECTDATA_H
class RectData
{
public:
    static float span;
    static float* vdata;    
    static int dataByteCount;
    static int vCount;
    static void  genVertexData();
};
#endif 
