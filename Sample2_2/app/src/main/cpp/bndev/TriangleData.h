#ifndef VULKANEXBASE_BALLDATA_H
#define VULKANEXBASE_BALLDATA_H

class TriangleData {
public:
    static float *vdata;
    static int dataByteCount;
    static int vCount;

    static void genVertexData();
};

#endif
