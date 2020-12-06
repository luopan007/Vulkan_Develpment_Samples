#ifndef VULKANEXBASE_BALLDATA_H
#define VULKANEXBASE_BALLDATA_H

//水面每个格子的尺寸
#define  SPAN 4
//水面的行列数
#define CR 127

class VertData
{
public:
    //顶点数据
    static float* vdata;
    //顶点数据字节数
    static int dataByteCount;
    //顶点数
    static int vCount;

    //索引数据
    static int* indexData;
    //索引数据字节数
    static int indexDataByteCount;
    //索引数
    static int indexCount;

    static void  genVertData();
};


#endif
