#ifndef MatrixState_hpp
#define MatrixState_hpp                                //防止重复定义

#include "Matrix.h"                                    //导入需要的头文件

class MatrixState3D {
public:
    static float currMatrix[16];                        //当前基本变换矩阵
    static float mProjMatrix[16];                        //投影矩阵
    static float mVMatrix[16];                            //摄像机观察矩阵
    static float mMVPMatrix[16];                        //总变换矩阵
    static float vulkanClipMatrix[16];                        //Vulkan专用标准设备空间调整矩阵
    static float mStack[10][16];                            //保存基本变换矩阵的栈
    static int stackTop;                                //栈顶索引
    static float cx, cy, cz;                                //摄像机位置坐标
    static void setInitStack();                            //初始化基本变换矩阵的方法
    static void pushMatrix();                            //保存基本变换矩阵入栈(保护现场)的方法
    static void popMatrix();                            //从栈恢复基本变换矩阵(恢复现场)的方法
    static void translate(float x, float y, float z);                //执行平移变换的方法
    static void rotate(float angle, float x, float y, float z);        //执行旋转变换的方法
    static void scale(float x, float y, float z);                    //执行缩放变换的方法
    static void setCamera(                            //设置摄像机的方法
            float cx, float cy, float cz,                            //摄像机位置坐标
            float tx, float ty, float tz,                            //目标点坐标
            float upx, float upy, float upz                        //摄像机UP向量
    );

    static void setProjectFrustum(                        //设置透视投影参数的方法
            float left, float right,                                //透视投影左、右参数
            float bottom, float top,                            //透视投影下、上参数
            float near, float far                                //透视投影近、远参数
    );

    static void setProjectOrtho(                        //设置正交投影参数的方法
            float left, float right,                                //正交投影左、右参数
            float bottom, float top,                            //正交投影下、上参数
            float near, float far                                //正交投影近、远参数
    );

    static float *getFinalMatrix();                        //获取总变换矩阵的方法
    static float *getMMatrix();                            //获取当前基本变换矩阵的方法
};

#endif
