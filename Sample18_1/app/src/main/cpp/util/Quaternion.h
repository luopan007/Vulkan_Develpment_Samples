#ifndef Quaternion_h
#define Quaternion_h
#include "Vector3f.h"
#include <math.h>

class Quaternion {
public :
	float w, x, y, z; //四元数的四个分量值
	static Quaternion* getIdentityQuaternion(); //获取单位四元数的方法
	Quaternion(float w, float x, float y, float z); //构造函数
	void setToRotateAboutAxis(Vector3f* axis, float theta);//构造表示绕指定轴旋转指定角度四元数的方法
	float getRotationAngle(); //提取旋转角的方法
	void getRotationAxis(Vector3f* axis); //提取旋转轴的方法
	void cross(Quaternion* a, Quaternion* result); //四元数叉乘运算，用以叠加多次旋转
};

#endif // !Quaternion_h
