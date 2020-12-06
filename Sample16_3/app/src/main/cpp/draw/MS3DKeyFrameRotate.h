/*
 * MS3DKeyFrameRotate.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#ifndef MS3DKEYFRAMEROTATE_H_
#define MS3DKEYFRAMEROTATE_H_
#include "../element/Vector4f.h"
class MS3DKeyFrameRotate{
private:
float time; //关键帧时间(单位为秒)
	Vector4f* rotate; //指向存储旋转数据复合数对象的指针
public:
	MS3DKeyFrameRotate(unsigned char* binaryData,int* binaryData_index);//构造函数
	~MS3DKeyFrameRotate(); //析构函数
	float getTime(); //获取关键帧时间的方法
	Vector4f* getRotate(); //获取关键帧旋转数据的方法
};
#endif