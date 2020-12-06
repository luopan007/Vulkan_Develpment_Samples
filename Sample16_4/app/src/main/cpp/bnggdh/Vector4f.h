#ifndef _VECTOR4F_H_
#define _VECTOR4F_H_
class Vector4f {
private:
public:
	float* coording;
	Vector4f();
	/*
	*@param    x1	用于构建Vector4f对象的X分量
	*@param    x2	用于构建Vector4f对象的y分量
	*@param    x3	用于构建Vector4f对象的z分量
	*@param    x4	用于构建Vector4f对象的w分量
	*/
	Vector4f(float x1, float x2, float x3, float x4);
	~Vector4f();
	void setFromEulerAngleToQuaternion(float yaw, float pitch, float roll);
	void interpolate(Vector4f* v1, Vector4f* v2, float alpha);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setW(float w);
	void setXYZW(float x, float y, float z, float w);
	float getX();
	float getY();
	float getZ();
	float getW();
};
#endif
