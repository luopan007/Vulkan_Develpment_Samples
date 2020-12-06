/*
 * MS3DKeyFramePosition.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#ifndef MS3DKEYFRAMEPOSITION_H_
#define MS3DKEYFRAMEPOSITION_H_
#include "../element/Vector3f.h"
class MS3DKeyFramePosition{
private:
	float time; //关键帧时间(单位为秒)
	Vector3f* position; //指向存储平移数据的复合数对象的指针
public:
	MS3DKeyFramePosition(unsigned char* binaryData,int* binaryData_index); //构造函数
	~MS3DKeyFramePosition(); //析构函数
	float getTime(); //获取关键帧时间的方法
	Vector3f* getPosition(); //获取关键帧平移数据的方法
};
#endif
