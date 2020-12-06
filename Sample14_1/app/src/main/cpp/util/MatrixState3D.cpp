#include "MatrixState3D.h"
#include <math.h>

float MatrixState3D::vulkanClipMatrix[16];
float MatrixState3D::currMatrix[16];
float MatrixState3D::mProjMatrix[16];
float MatrixState3D::mVMatrix[16];
float MatrixState3D::mMVPMatrix[16];
float MatrixState3D::mStack[10][16];
int MatrixState3D::stackTop=-1;

void MatrixState3D::setInitStack()
{
	Matrix::setIdentityM(currMatrix,0);

	//初始化从OpenGL标准设备空间到Vulkan设备空间变换的矩阵
	//OpenGL标准设备空间XYZ三个轴范围都是从-1.0～+1.0
	//Vulkan设备空间XYZ三个轴范围分别是 -1.0～+1.0、+1.0～-1.0、0.0～+1.0
	//变换时本质上采用的是缩放加平移矩阵
	//X轴不变Y轴置反Z轴缩放0.5
	//缩放后Z轴正向平移0.5
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




