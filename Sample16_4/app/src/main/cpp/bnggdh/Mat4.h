#ifndef _MAT4_H_
#define _MAT4_H_
#include "Vector3f.h"
#include "Vector4f.h"
class Mat4{
public:
	float* matrix;
	float* mulHelper;
	Mat4();
	~Mat4();
	void loadIdentity();
	void setTranslation(Vector3f* v);
	void genRotationFromEulerAngle(Vector3f* angles);
	Mat4* mul(Mat4* m1, Mat4* m2);
	void copyFrom(Mat4* m);
	void genRotateFromQuaternion(Vector4f* v);
	void invTransformAndRotate(Vector3f* point, Vector3f* v);
	void transform(Vector3f* point, Vector3f* v);
	void translate(float x, float y, float z);
	void rotate(float angle, float x, float y, float z);
	void scale(float x, float y, float z);
	static void print_matrix(Mat4 m);
};
#endif
