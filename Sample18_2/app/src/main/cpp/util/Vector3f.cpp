#include "Vector3f.h"
#include <math.h>
Vector3f::Vector3f() {}
Vector3f::Vector3f(float x,float y,float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
void Vector3f::add(Vector3f* v, Vector3f* result) {
	result->x = this->x + v->x;
	result->y = this->y + v->y;
	result->z = this->z + v->z;
}
void Vector3f::minus(Vector3f* v, Vector3f* result) {
	result->x = this->x - v->x;
	result->y = this->y - v->y;
	result->z = this->z - v->z;
}
void Vector3f::multiK(Vector3f* v, Vector3f* result) {
	result->x = this->x * v->x;
	result->y = this->y * v->y;
	result->z = this->z * v->z;
}
void Vector3f::normalize() {
	float mod = module();
	this->x /= mod;
	this->y /= mod;
	this->z /= mod;
}
float Vector3f::module() {
	return (float)sqrt(x*x + y*y + z*z);
}
