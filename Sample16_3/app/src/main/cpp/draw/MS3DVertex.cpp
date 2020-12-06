/*
 * MS3DVertex.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DVertex.h"
#include <../util/FileUtil.h>
#include<fstream>
MS3DVertex::MS3DVertex(unsigned char* binaryData,int* binaryData_index){ //构造函数
	currPosition = new Vector3f(); //创建表示顶点当前位置的复合数对象
	FileUtil::myReadByte(binaryData, binaryData_index);//读取顶点在编辑器中的状态，本案例中无用
	float x =FileUtil::myReadFloat(binaryData, binaryData_index); //读取顶点初始位置坐标的X 分量
	float y =FileUtil::myReadFloat(binaryData, binaryData_index); //读取顶点初始位置坐标的Y 分量
	float z =FileUtil::myReadFloat(binaryData, binaryData_index); //读取顶点初始位置坐标的Z 分量
	initPosition =new Vector3f(x,y,z); //创建表示顶点初始位置的复合数对象
	bone = FileUtil::myReadByte(binaryData, binaryData_index); //读取影响该顶点的骨骼(关节)索引
	FileUtil::myReadByte(binaryData, binaryData_index); //读取标志，本案例中无用
}
Vector3f* MS3DVertex::getInitPosition(){ //获取顶点初始位置的方法
	return initPosition; //返回存储顶点初始位置的复合数对象的指针
}
int MS3DVertex::getBone(){ //获取影响该顶点的骨骼(关节)索引的方法
	return bone; //返回影响该顶点的骨骼(关节)索引
}
Vector3f* MS3DVertex::getCurrPosition(){ //获取顶点当前位置的方法
	return currPosition; //返回存储顶点当前位置的复合数对象的指针
}
void MS3DVertex::setCurrPosition(Vector3f* buffer) { //设置顶点当前位置的方法
	currPosition->setX(buffer->getX()); //设置顶点当前位置坐标的X 分量
	currPosition->setY(buffer->getY()); //设置顶点当前位置坐标的Y 分量
	currPosition->setZ(buffer->getZ()); //设置顶点当前位置坐标的Z 分量
}
MS3DVertex::~MS3DVertex(){
	delete initPosition;
	delete currPosition;
}
