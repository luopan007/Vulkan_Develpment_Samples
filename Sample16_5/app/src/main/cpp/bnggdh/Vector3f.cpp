#include "Vector3f.h"
Vector3f::Vector3f() {
	coord=new float[3]();
}
Vector3f::Vector3f(float x, float y, float z) {
	coord=new float[3]{x,y,z};
}
Vector3f::~Vector3f()
{
	delete[] coord;
}
void Vector3f::mulScale(float scale) {
	this->coord[0] *= scale;
	this->coord[1] *= scale;
	this->coord[2] *= scale;
}
void Vector3f::interpolate(Vector3f* v1, Vector3f* v2, float alpha) {
	this->setX((1 - alpha) * v1->getX() + alpha * v2->getX());
	this->setY((1 - alpha) * v1->getY() + alpha * v2->getY());
	this->setZ((1 - alpha) * v1->getZ() + alpha * v2->getZ());
}
void Vector3f::interpolate_four(Vector3f* v0, Vector3f* v1, Vector3f* v2, Vector3f* v3, float weight0, float weight1, float weight2, float weight3) {
	this->setX(v0->getX() * weight0 + v1->getX() * weight1 + v2->getX() * weight2 + v3->getX() * weight3);
	this->setY(v0->getY() * weight0 + v1->getY() * weight1 + v2->getY() * weight2 + v3->getY() * weight3);
	this->setZ(v0->getZ() * weight0 + v1->getZ() * weight1 + v2->getZ() * weight2 + v3->getZ() * weight3);
}
float* Vector3f:: getVector3fArray()
{
	return this->coord;
}
void Vector3f::setX(float x) {
	this->coord[0] = x;
}
void Vector3f::setY(float y) {
	this->coord[1] = y;
}
void Vector3f::setZ(float z) {
	this->coord[2] = z;
}
void Vector3f::setXYZ(float x, float y, float z) {
	this->coord[0] = x;
	this->coord[1] = y;
	this->coord[2] = z;
}
float Vector3f::getX()
{
	return this->coord[0];
}
float Vector3f::getY()
{
	return this->coord[1];
}
float Vector3f::getZ()
{
	return this->coord[2];
}
