#include "MatrixState3D.h"
#include "Vector3f.h"
#include <math.h>
float MatrixState3D::vulkanClipMatrix[16];
float MatrixState3D::currMatrix[16];
float MatrixState3D::mProjMatrix[16];
float MatrixState3D::mVMatrix[16];
float MatrixState3D::mMVPMatrix[16];
float MatrixState3D::mStack[10][16];
float MatrixState3D::invertMvMatrix[16];
float MatrixState3D::invertMMatrix[16];
float MatrixState3D::helpArr[4];
float MatrixState3D::helpResult[4];
int MatrixState3D::stackTop=-1;
float MatrixState3D::cx,MatrixState3D::cy,MatrixState3D::cz;
float MatrixState3D::left;
float MatrixState3D::right;
float MatrixState3D::bottom;
float MatrixState3D::top;
float MatrixState3D::nearSelf;
float MatrixState3D::farSelf;
void MatrixState3D::setInitStack()
{
	Matrix::setIdentityM(currMatrix,0);
	vulkanClipMatrix[0]=1.0f;
	vulkanClipMatrix[1]=0.0f;
	vulkanClipMatrix[2]=0.0f;
	vulkanClipMatrix[3]=0.0f;
	vulkanClipMatrix[4]=0.0f;
	vulkanClipMatrix[5]=-1.0f;
	vulkanClipMatrix[6]=0.0f;
	vulkanClipMatrix[7]=0.0f;
	vulkanClipMatrix[8]=0.0f;
	vulkanClipMatrix[9]=0.0f;
	vulkanClipMatrix[10]=0.5f;
	vulkanClipMatrix[11]=0.0f;
	vulkanClipMatrix[12]=0.0f;
	vulkanClipMatrix[13]=0.0f;
	vulkanClipMatrix[14]=0.5f;
	vulkanClipMatrix[15]=1.0f;
}
void MatrixState3D::pushMatrix()
{
	stackTop++;
	for(int i=0;i<16;i++)
	{
		mStack[stackTop][i]=currMatrix[i];
	}
}
void MatrixState3D::popMatrix()
{
	for(int i=0;i<16;i++)
	{
		currMatrix[i]=mStack[stackTop][i];
	}
	stackTop--;
}
void MatrixState3D::translate(float x,float y,float z)
{
	Matrix::translateM(currMatrix, 0, x, y, z);
}
void MatrixState3D::rotate(float angle,float x,float y,float z)
{
	Matrix::rotateM(currMatrix,0,angle,x,y,z);
}
void MatrixState3D::scale(float x,float y,float z)
{
	Matrix::scaleM(currMatrix,0, x, y, z);
}
void MatrixState3D::setCamera
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
 )
{
	MatrixState3D::cx=cx;
	MatrixState3D::cy=cy;
	MatrixState3D::cz=cz;
	Matrix::setLookAtM
	(
	 mVMatrix,
	 0,
	 cx,
	 cy,
	 cz,
	 tx,
	 ty,
	 tz,
	 upx,
	 upy,
	 upz
	 );\
}
void MatrixState3D::setProjectFrustum
(
 float left,
 float right,
 float bottom,
 float top,
 float near,
 float far
 )
{
    MatrixState3D::left = left;
    MatrixState3D::right = right;
    MatrixState3D::bottom = bottom;
    MatrixState3D::top = top;
    MatrixState3D::nearSelf = near;
    MatrixState3D::farSelf = far;
	Matrix::frustumM(mProjMatrix, 0, left, right, bottom, top, near, far);
}
float* MatrixState3D::getFinalMatrix()
{
	Matrix::multiplyMM(mMVPMatrix, 0, mVMatrix, 0, currMatrix, 0);
	Matrix::multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
	Matrix::multiplyMM(mMVPMatrix, 0, vulkanClipMatrix, 0, mMVPMatrix, 0);
	return mMVPMatrix;
}
float* MatrixState3D::getMMatrix()
{
	return currMatrix;
}
void MatrixState3D::fromPtoPreP //将摄像机坐标系中坐标变换为世界坐标系中坐标的方法
        (float x, float y, float z, float* result){
    Matrix::invertM(invertMvMatrix, 0, mVMatrix, 0); //求摄像机矩阵的逆矩阵
    helpArr[0] = x;helpArr[1] = y;helpArr[2] = z;helpArr[3] = 1; //将需要变换的点坐标转存进辅助数组
    Matrix::multiplyMV(result, 0, invertMvMatrix, 0, helpArr, 0);//求变换之前世界坐标系中的坐标
}
Vector3f* MatrixState3D::fromGToO(Vector3f* v,float* m) {//将世界坐标系中坐标变换为物体坐标系中坐标
    Matrix::invertM(invertMMatrix, 0, m, 0); //求基本变换矩阵的逆矩阵
    helpArr[0]=v->x;helpArr[1]=v->y;helpArr[2]=v->z;helpArr[3]=1; //将需变换的点坐标转存进辅助数组
    Matrix::multiplyMV(helpResult, 0, invertMMatrix, 0, helpArr, 0); //求变换之前物体坐标系中的坐标
    return new Vector3f(helpResult[0], helpResult[1], helpResult[2]);//返回变换后物体坐标系中的坐标
}