#include <math.h>
#include <cstdio>
#include "Vector4f.h"
Vector4f::Vector4f() {
	coording=new float[4]();
}
Vector4f::Vector4f(float x1, float x2, float x3, float x4)
{
	coording=new float[4]{x1,x2,x3,x4};
}
Vector4f::~Vector4f()
{
	delete[] coording;
}
void Vector4f::setFromEulerAngleToQuaternion(float yaw, float pitch, float roll) {
	float angle = 0.0F;
	angle = yaw * 0.5F;
	float sr = (float)sin((double)angle);
	float cr = (float)cos((double)angle);
	angle = pitch * 0.5F;
	float sp = (float)sin((double)angle);
	float cp = (float)cos((double)angle);
	angle = roll * 0.5F;
	float sy = (float)sin((double)angle);
	float cy = (float)cos((double)angle);
	this->setX(sr * cp * cy - cr * sp * sy);
	this->setY(cr * sp * cy + sr * cp * sy);
	this->setZ(cr * cp * sy - sr * sp * cy);
	this->setW(cr * cp * cy + sr * sp * sy);
}
void Vector4f::interpolate(Vector4f* v1, Vector4f* v2, float alpha) {
	double dot = 0.0;
	for(int v0 = 0; v0 < 4; ++v0) {
		dot += (double)(v2->coording[v0] * v1->coording[v0]);
	}
	if (dot < 0.0)
	{
		for (int i = 0; i < 4; ++i) {
			v1->coording[i] = -v1->coording[i];
		}
		dot = -dot;
	}
	double s1;
	double s2;
	if(dot > 0.999999) {
		s1 = 1.0 - (double)alpha;
		s2 = (double)alpha;
	} else {
		double om = acos(dot);
		double sinom = sin(om);
		s1 = sin((1.0 - (double)alpha) * om) / sinom;
		s2 = sin((double)alpha * om) / sinom;
	}
	this->setW((float)(s1 * (double)v1->getW() + s2 * (double)v2->getW()));
	this->setX((float)(s1 * (double)v1->getX() + s2 * (double)v2->getX()));
	this->setY((float)(s1 * (double)v1->getY() + s2 * (double)v2->getY()));
	this->setZ((float)(s1 * (double)v1->getZ() + s2 * (double)v2->getZ()));
}
void Vector4f::setX(float x) 
{
	this->coording[0] = x;
}
void Vector4f::setY(float y) {
	this->coording[1] = y;
}
void Vector4f::setZ(float z) {
	this->coording[2] = z;
}
void Vector4f::setW(float w) {
	this->coording[3] = w;
}
void Vector4f::setXYZW(float x, float y, float z, float w)
{
	this->coording[0] = x;
	this->coording[1] = y;
	this->coording[2] = z;
	this->coording[3] = w;
}
float Vector4f::getX() {
	return this->coording[0];
}
float Vector4f::getY() {
	return this->coording[1];
}
float Vector4f::getZ() {
	return this->coording[2];
}
float Vector4f::getW() {
	return this->coording[3];
}
