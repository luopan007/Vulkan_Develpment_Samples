#ifndef MatrixState_hpp//防止重复定义
#define MatrixState_hpp

#include "Matrix.h"//导入需要的头文件

class MatrixState3D
{
public:
    static float cx;
    static float cy;
    static float cz;
    static float mProjMatrix[16];//投影矩阵
    static float mVMatrix[16];//摄像机矩阵
    static float vulkanClipMatrix[16];//剪裁空间矩阵 X不变 Y置反 Z减半
    static void setInitStack(float* currMatrix);//初始化矩阵

    static void translate(float* currMatrix,float x,float y,float z);//沿x、y、z轴平移

    static void rotate(float* currMatrix,float angle,float x,float y,float z);//绕指定轴旋转

    static void scale(float* currMatrix,float x,float y,float z);//矩阵缩放


    static void setCamera//设置摄像机
            (
                    float cx,
                    float cy,
                    float cz,
                    float tx,
                    float ty,
                    float tz,
                    float upx,
                    float upy,
                    float upz
            );

    static void setProjectFrustum//设置透视投影参数
            (
                    float left,
                    float right,
                    float bottom,
                    float top,
                    float near,
                    float far
            );

    static void getFinalMatrix(float* mMVPMatrix,float* currMatrix);//获取最终矩阵
};


#endif
