#pragma once
#ifndef LFTData_H
#define LFTData_H
#define CUBE_W 1.0f
#define CUBE_H 1.2f
#define CUBE_D 1.0f
class LFTData {
public:
    static float* vdata;
    static int dataByteCount;
    static int vCount;
    static void getVData();
};
#endif 
