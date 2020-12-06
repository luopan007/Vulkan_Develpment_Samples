#include "Mat4.h"
#include "../util/Matrix.h"
#include "stdio.h"
#include <string.h>
#include <math.h>
#include <mylog.h>

Mat4::Mat4(){
	matrix = new float[16]();
	mulHelper = new float[16]();
}
Mat4::~Mat4()
{
	delete[] matrix;
	delete[] mulHelper;
}
void Mat4::loadIdentity()
{
	Matrix::setIdentityM(this->matrix, 0);
}
void Mat4::setTranslation(Vector3f* v) 
{
	this->matrix[12] = v->getX();
	this->matrix[13] = v->getY();
	this->matrix[14] = v->getZ();
}
void Mat4::copyFrom(Mat4* m)
{
	memcpy(this->matrix, m->matrix,sizeof(float)*16);
}
void Mat4::genRotationFromEulerAngle(Vector3f* angles)
{
		float angle = angles->getX() * 0.5F;
		float sin0 = (float)sin((double)angle);
		float cos0 = (float)cos((double)angle);
		angle = angles->getY() * 0.5F;
		float sin1 = (float)sin((double)angle);
		float cos1 = (float)cos((double)angle);
		angle = angles->getZ() * 0.5F;
		float sin2 = (float)sin((double)angle);
		float cos2 = (float)cos((double)angle);
		float x = sin0 * cos1 * cos2 - cos0 * sin1 * sin2;
		float y = cos0 * sin1 * cos2 + sin0 * cos1 * sin2;
		float z = cos0 * cos1 * sin2 - sin0 * sin1 * cos2;
		float w = cos0 * cos1 * cos2 + sin0 * sin1 * sin2;
		this->matrix[0] = 1.0F - 2.0F * y * y - 2.0F * z * z;
		this->matrix[1] = 2.0F * x * y + 2.0F * w * z;
		this->matrix[2] = 2.0F * x * z - 2.0F * w * y;
		this->matrix[4] = 2.0F * x * y - 2.0F * w * z;
		this->matrix[5] = 1.0F - 2.0F * x * x - 2.0F * z * z;
		this->matrix[6] = 2.0F * y * z + 2.0F * w * x;
		this->matrix[8] = 2.0F * x * z + 2.0F * w * y;
		this->matrix[9] = 2.0F * y * z - 2.0F * w * x;
		this->matrix[10] = 1.0F - 2.0F * x * x - 2.0F * y * y;
}
Mat4* Mat4::mul(Mat4* m1, Mat4* m2)
{
	Matrix::multiplyMM(mulHelper, 0, m1->matrix, 0, m2->matrix, 0);
	memcpy(matrix, mulHelper,sizeof(float)*16);	
	return this;
}
void Mat4::genRotateFromQuaternion(Vector4f* v)
{
	float x = v->getX();
	float y = v->getY();
	float z = v->getZ();
	float w = v->getW();
	this->matrix[0] = 1.0F - 2.0F * y * y - 2.0F * z * z;
	this->matrix[1] = 2.0F * (x * y + w * z);
	this->matrix[2] = 2.0F * (x * z - w * y);
	this->matrix[4] = 2.0F * (x * y - w * z);
	this->matrix[5] = 1.0F - 2.0F * x * x - 2.0F * z * z;
	this->matrix[6] = 2.0F * (y * z + w * x);
	this->matrix[8] = 2.0F * (x * z + w * y);
	this->matrix[9] = 2.0F * (y * z - w * x);
	this->matrix[10] = 1.0F - 2.0F * x * x - 2.0F * y * y;
	this->matrix[12] = 0.0F;
	this->matrix[13] = 0.0F;
	this->matrix[14] = 0.0F;
	this->matrix[3] = 0.0F;
	this->matrix[7] = 0.0F;
	this->matrix[11] = 0.0F;
	this->matrix[15] = 1.0F;
}
void Mat4::invTransformAndRotate(Vector3f* point, Vector3f* v)
{
	float x = point->getX() - this->matrix[12];
	float y = point->getY() - this->matrix[13];
	float z = point->getZ() - this->matrix[14];
	v->setX(this->matrix[0] * x + this->matrix[1] * y + this->matrix[2] * z);
	v->setY(this->matrix[4] * x + this->matrix[5] * y + this->matrix[6] * z);
	v->setZ(this->matrix[8] * x + this->matrix[9] * y + this->matrix[10] * z);
}
void Mat4::transform(Vector3f* point, Vector3f* v) 
{
	v->setX(this->matrix[0] * point->getX() + this->matrix[4] * point->getY() + this->matrix[8] * point->getZ() + this->matrix[12]);
	v->setY(this->matrix[1] * point->getX() + this->matrix[5] * point->getY() + this->matrix[9] * point->getZ() + this->matrix[13]);
	v->setZ(this->matrix[2] * point->getX() + this->matrix[6] * point->getY() + this->matrix[10] * point->getZ() + this->matrix[14]);
}
void Mat4::translate(float x, float y, float z)
{
	Matrix::translateM(matrix, 0, x, y, z);
}
void Mat4::rotate(float angle, float x, float y, float z)
{
	Matrix::rotateM(matrix, 0, angle, x, y, z);
}
void Mat4::scale(float x, float y, float z)
{
	Matrix::scaleM(this->matrix, 0, x, y, z);
}
void Mat4::print_matrix(Mat4 m)
{
	for(int i = 0; i < 4; i++)
	{
		LOGE("%f,%f,%f,%f\n",m.matrix[i*4],m.matrix[i*4+1],m.matrix[i*4+2],m.matrix[i*4+3]);
	}
}
