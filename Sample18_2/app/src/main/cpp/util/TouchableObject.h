#pragma once
#ifndef TouchableObject_H
#define TouchableObject_H
#include "AABB.h"
#include "MatrixState3D.h"
class TouchableObject {
public:
	~TouchableObject(); //析构函数
	AABB* preBox; //指向当前物体对应AABB 包围盒对象的指针
	float* m=new float[16]; //指向当前基本变换矩阵数组首地址的指针
	float* color = new float[4]{1,1,1,1}; //顶点颜色
	float size=1.5f; //当前物体绘制时的放大率
	AABB* getCurrBox(); //获得物体AABB 包围盒的方法
	void changeOnTouch(bool flag); //根据选中标志改变物体颜色、尺寸的方法
	void copyM(); //复制当前基本变换矩阵的方法
};
#endif