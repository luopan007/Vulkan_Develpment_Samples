#ifndef _VECTOR3F_H_
#define _VECTOR3F_H_
class Vector3f {
public:
	float* coord;
public:
	Vector3f();
	Vector3f(float x, float y, float z);
	~Vector3f();
	void interpolate(Vector3f* v1, Vector3f* v2, float alpha);
	float* getVector3fArray();
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	float getX();
	float getY();
	float getZ();
};
#endif
