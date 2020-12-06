/*
 * MS3DKeyFramePosition.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DKeyFramePosition.h"
#include <../util/FileUtil.h>
MS3DKeyFramePosition::MS3DKeyFramePosition(unsigned char* binaryData,int* binaryData_index){
	time = FileUtil::myReadFloat(binaryData,binaryData_index); //读取关键帧时间
	float x = FileUtil::myReadFloat(binaryData,binaryData_index); //读取平移信息的x 分量
	float y = FileUtil::myReadFloat(binaryData,binaryData_index); //读取平移信息的y 分量
	float z = FileUtil::myReadFloat(binaryData,binaryData_index); //读取平移信息的z 分量
	position =new Vector3f(x,y,z); //创建存储平移信息的复合数对象
}
MS3DKeyFramePosition::~MS3DKeyFramePosition(){ //析构函数
	delete position; //释放存储平移信息的复合数对象
}
float MS3DKeyFramePosition::getTime(){ //获取关键帧时间的方法
	return time; //返回此关键帧的时间
}
Vector3f* MS3DKeyFramePosition::getPosition(){ //获取关键帧平移数据的方法
	return position; //返回此关键帧的平移数据
}
