#ifndef VULKANEXBASE_STARDATA_H
#define VULKANEXBASE_STARDATA_H                            //防止重复定义

class SixPointedStar {
public:
    static float *vdata;                                            //顶点数据指针
    static int dataByteCount;                                    //顶点数据字节数量
    static int vCount;                                                //顶点数量
    static void genStarData(float R, float r, float z);                        //计算六角形数据的方法
};

#endif
