#ifndef SAMPLE5_1_BALLDATA_H
#define SAMPLE5_1_BALLDATA_H
class BallData
{
public:
    static float* vdata;
    static int dataByteCount;
    static int vCount;
    static void  genBallData(float angleSpan);
};
#endif 
