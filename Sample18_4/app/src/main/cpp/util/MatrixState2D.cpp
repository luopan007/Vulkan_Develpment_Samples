#include "MatrixState2D.h"
#include <math.h>
float MatrixState2D::vulkanClipMatrix[16];
float MatrixState2D::currMatrix[16];
float MatrixState2D::mProjMatrix[16];
float MatrixState2D::mVMatrix[16];
float MatrixState2D::mMVPMatrix[16];
float MatrixState2D::mStack[10][16];
int MatrixState2D::stackTop = -1;
void MatrixState2D::setInitStack()
{
	Matrix::setIdentityM(currMatrix, 0);
	vulkanClipMatrix[0] = 1.0f;
	vulkanClipMatrix[1] = 0.0f;
	vulkanClipMatrix[2] = 0.0f;
	vulkanClipMatrix[3] = 0.0f;
	vulkanClipMatrix[4] = 0.0f;
	vulkanClipMatrix[5] = -1.0f;
	vulkanClipMatrix[6] = 0.0f;
	vulkanClipMatrix[7] = 0.0f;
	vulkanClipMatrix[8] = 0.0f;
	vulkanClipMatrix[9] = 0.0f;
	vulkanClipMatrix[10] = 0.5f;
	vulkanClipMatrix[11] = 0.0f;
	vulkanClipMatrix[12] = 0.0f;
	vulkanClipMatrix[13] = 0.0f;
	vulkanClipMatrix[14] = 0.5f;
	vulkanClipMatrix[15] = 1.0f;
}
void MatrixState2D::pushMatrix()
{
	stackTop++;
	for (int i = 0; i<16; i++)
	{
		mStack[stackTop][i] = currMatrix[i];
	}
}
void MatrixState2D::popMatrix()
{
	for (int i = 0; i<16; i++)
	{
		currMatrix[i] = mStack[stackTop][i];
	}
	stackTop--;
}
void MatrixState2D::translate(float x, float y, float z)
{
	Matrix::translateM(currMatrix, 0, x, y, z);
}
void MatrixState2D::rotate(float angle, float x, float y, float z)
{
	Matrix::rotateM(currMatrix, 0, angle, x, y, z);
}
void MatrixState2D::scale(float x, float y, float z)
{
	Matrix::scaleM(currMatrix, 0, x, y, z);
}
void MatrixState2D::setCamera
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
			); \
}
void MatrixState2D::setProjectOrtho
		(
				float left,
				float right,
				float bottom,
				float top,
				float near,
				float far
		)
{
	Matrix::orthoM(mProjMatrix, 0, left, right, bottom, top, near, far);
}
float* MatrixState2D::getFinalMatrix()
{
	Matrix::multiplyMM(mMVPMatrix, 0, mVMatrix, 0, currMatrix, 0);
	Matrix::multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
	Matrix::multiplyMM(mMVPMatrix, 0, vulkanClipMatrix, 0, mMVPMatrix, 0);
	return mMVPMatrix;
}
float* MatrixState2D::getMMatrix()
{
	return currMatrix;
}
