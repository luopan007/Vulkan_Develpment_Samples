#pragma once
#ifndef Vector3f_H
#define Vector3f_H
class Vector3f {
public :
	float x = 0;
	float y = 0;
	float z = 0;
	Vector3f();
	Vector3f(float x, float y, float z);
	void add(Vector3f* v, Vector3f* result);
	void minus(Vector3f* v, Vector3f* result);
	void multiK(Vector3f* v, Vector3f* result);
	void normalize();
	float module();
};
#endif