#ifndef SAMPLE11_3_TREELEAVES_H
#define SAMPLE11_3_TREELEAVES_H
class TreeLeaves {
public:
    static float* vdata;
    static int dataByteCount;
    static int vCount;
    static void genVertexData(float width,float height,float absolute_height);//生成树叶的方法
};
#endif //SAMPLE11_3_TREELEAVES_H
