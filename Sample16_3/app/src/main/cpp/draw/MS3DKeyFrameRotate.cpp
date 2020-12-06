/*
 * MS3DKeyFrameRotate.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DKeyFrameRotate.h"
#include <../util/FileUtil.h>
MS3DKeyFrameRotate::MS3DKeyFrameRotate(unsigned char* binaryData,int* binaryData_index){
	time =FileUtil::myReadFloat(binaryData,binaryData_index); //读取关键帧时间
	rotate = new Vector4f(); //创建存储旋转数据的复合数对象
	float x = FileUtil::myReadFloat(binaryData,binaryData_index); //读取旋转欧拉角的第1 个分量
	float y = FileUtil::myReadFloat(binaryData,binaryData_index); //读取旋转欧拉角的第2 个分量
	float z = FileUtil::myReadFloat(binaryData,binaryData_index); //读取旋转欧拉角的第3 个分量
	rotate-> //将欧拉角形式的旋转数据转换为四元数形式（这是为了在关键帧之间进行插值计算）
			setFromEulerAngleToQuaternion(x,y,z);
}
MS3DKeyFrameRotate::~MS3DKeyFrameRotate(){ //析构函数
	delete rotate; //释放存储旋转数据的复合数对象
}
float MS3DKeyFrameRotate::getTime(){ //获取关键帧时间的方法
	return time; //返回此关键帧的时间
}
Vector4f* MS3DKeyFrameRotate::getRotate(){ //获取关键帧旋转数据的方法
	return rotate; //返回关键帧旋转数据
}
