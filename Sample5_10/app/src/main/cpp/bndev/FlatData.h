#ifndef VULKANEXBASE_BALLDATA_H
#define VULKANEXBASE_BALLDATA_H //防止重复引用
class FlatData{
public:
    static float* vdata; //数据数组首地址指针
    static int dataByteCount; //数据所占总字节数
    static int vCount; //顶点数量
    static void genData(); //生成顶点数据的方法
};
#endif
