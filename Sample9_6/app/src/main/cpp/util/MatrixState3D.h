#ifndef MatrixState_hpp
#define MatrixState_hpp
#include "Matrix.h"
class MatrixState3D
{
public:
    static float currMatrix[16];
	static float mProjMatrix[16];
    static float mVMatrix[16];
    static float mMVPMatrix[16];
    static float vulkanClipMatrix[16];
    static float mStack[10][16];
    static int stackTop;
    static float cx,cy,cz;
    static void setInitStack();
    static void pushMatrix();
    static void popMatrix();
    static void translate(float x,float y,float z);
    static void rotate(float angle,float x,float y,float z);
    static void scale(float x,float y,float z);
    static void setCamera
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
    static void setProjectFrustum
    (
     float left,
     float right,
     float bottom,
     float top,
     float near,
     float far
     );
    static float* getFinalMatrix();
    static float* getMMatrix();
};
#endif
