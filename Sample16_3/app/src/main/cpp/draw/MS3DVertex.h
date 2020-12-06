/*
 * MS3DVertex.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#ifndef MS3DVERTEX_H_
#define MS3DVERTEX_H_
#include "../element/Vector3f.h"
class MS3DVertex{
private:
	Vector3f* initPosition; //从文件中读取的顶点原始坐标
	Vector3f* currPosition; //动画中实时变化的顶点坐标
	int bone; //影响该顶点的骨骼（关节）索引
public:
	MS3DVertex(unsigned char* binaryData,int* binaryData_index); //构造函数
	~MS3DVertex(); //析构函数
	Vector3f* getInitPosition(); //获取顶点初始位置坐标的方法
	Vector3f* getCurrPosition(); //获取顶点当前位置坐标的方法
	void setCurrPosition(Vector3f* buffer); //设置顶点当前位置坐标的方法
	int getBone(); //获取影响该顶点的骨骼(关节)索引的方法
};
#endif
