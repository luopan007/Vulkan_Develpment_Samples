#include "Quaternion.h"
Quaternion::Quaternion(float w,float x,float y,float z) { //构造函数
	this->w = w;this->x = x;this->y = y;this->z = z; //接收并保存四元数的四个分量值
}
Quaternion* Quaternion::getIdentityQuaternion(){ //获取单位四元数的方法
	return new Quaternion(1.0f, 0.0f, 0.0f, 0.0f); //创建并返回一个单位四元数对象
}
void Quaternion::setToRotateAboutAxis(Vector3f* axis,float theta){//构造绕指定轴旋转指定角四元数的方法
	axis->normalize(); //规格化旋转轴向量
	float thetaOver2 = theta / 2.0f; //计算旋转角的半角
	float sinThetaOver2 = (float)sin(thetaOver2); //计算半角的正弦值
	w = (float)cos(thetaOver2); //将半角余弦值赋给四元数的w 分量
	x = axis->x * sinThetaOver2; //计算四元数的x 分量并赋值
	y = axis->y * sinThetaOver2; //计算四元数的y 分量并赋值
	z = axis->z * sinThetaOver2; //计算四元数的z 分量并赋值
}
float Quaternion::getRotationAngle() { //获取旋转角的方法
	float thetaOver2 = (float)acos(w); //计算旋转角的半角值
	return thetaOver2 * 2.0f; //返回旋转角
}
void Quaternion::getRotationAxis(Vector3f* axis) { //提取旋转轴的方法
	float sinThetaOver2Sq = 1.0f - w*w; //计算半角正弦值的平方，依据公式sin^2(x)+cos^2(x)=1
	if (sinThetaOver2Sq <= 0.0f) { //判断计算结果是否在一般范围内
		axis->x = 1.0f; axis->y = 0.0f;axis->z = 0.0f; //若不在则指定一个有效向量
		return; //返回
	}
	float oneOversinThetaOver2 = (float)(1.0f / sqrt(sinThetaOver2Sq));//计算1/sin(theta/2)
	axis->x = x * oneOversinThetaOver2; //计算旋转轴的X 分量
	axis->y = y * oneOversinThetaOver2; //计算旋转轴的Y 分量
	axis->z = z * oneOversinThetaOver2; //计算旋转轴的Z 分量
}
void Quaternion::cross(Quaternion* a, Quaternion* result) { //实现四元数叉乘运算的方法
	float wTemp = w*a->w - x*a->x - y*a->y - z*a->z; //计算叉乘结果的w 分量值
	float xTemp = w*a->x + x*a->w + z*a->y - y*a->z; //计算叉乘结果的x 分量值
	float yTemp = w*a->y + y*a->w + x*a->z - z*a->x; //计算叉乘结果的y 分量值
	float zTemp = w*a->z + z*a->w + y*a->x - x*a->y; //计算叉乘结果的z 分量值
	result->w = wTemp; result->x = xTemp; //为结果四元数w、x 分量赋值
	result->y = yTemp; result->z = zTemp; //为结果四元数y、z 分量赋值
}