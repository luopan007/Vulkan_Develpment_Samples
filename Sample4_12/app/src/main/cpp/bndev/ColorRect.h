#ifndef VULKANEXBASE_COLORRECT_H
#define VULKANEXBASE_COLORRECT_H //防止重复定义
class ColorRect{
public:
    static float* vdataG; //青色正方形顶点数据指针
    static float* vdataY; //黄色正方形顶点数据指针
    static int dataByteCount; //每个正方形顶点数据所占总字节数
    static int vCount; //每个正方形顶点数量
    static float UNIT_SIZEG; //青色正方形边长
    static float UNIT_SIZEY; //黄色正方形边长
    static void genVertexData(); //生成顶点数据的方法
};
#endif
