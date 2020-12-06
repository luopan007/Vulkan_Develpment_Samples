#ifndef _VECTOR3F_H_
#define _VECTOR3F_H_
class Vector3f {
public:
	float* coord;
public:
	Vector3f();
	/*
	*@param    x	用于构建Vector3f对象的X分量
	*@param    y	用于构建Vector3f对象的y分量
	*@param    z	用于构建Vector3f对象的z分量
	*/
	Vector3f(float x, float y, float z);
	~Vector3f();
	void mulScale(float scale);
	void interpolate_four(Vector3f* v0, Vector3f* v1, Vector3f* v2, Vector3f* v3, float weight0, float weight1, float weight2, float weight3);
	void interpolate(Vector3f* v1, Vector3f* v2, float alpha);
	float* getVector3fArray();
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	/*
	*@param    x	X分量
	*@param    y	y分量
	*@param    z	z分量
	*/
	void setXYZ(float x, float y, float z);
	float getX();
	float getY();
	float getZ();
};
#endif
