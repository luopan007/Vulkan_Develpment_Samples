/*
 * MS3DTriangle.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DTriangle.h"
#include "../util/FileUtil.h"
#include <fstream>
MS3DTriangle::MS3DTriangle(unsigned char* binaryData,int* binaryData_index){
	FileUtil::myReadUnsignedShort(binaryData, binaryData_index);//读取三角形在编辑器中的状态值
	indexs = new int[3]{ //初始化三角形三个顶点索引的数组
			FileUtil::myReadUnsignedShort(binaryData, binaryData_index), //读取第一个顶点的索引
			FileUtil::myReadUnsignedShort(binaryData, binaryData_index), //读取第二个顶点的索引
			FileUtil::myReadUnsignedShort(binaryData, binaryData_index)}; //读取第三个顶点的索引
	for(int j=0; j<3; j++){ //对三角形中的3 个顶点进行遍历
		float nx=FileUtil::myReadFloat(binaryData, binaryData_index);//读取当前顶点法向量的X 分量
		float ny=FileUtil::myReadFloat(binaryData, binaryData_index);//读取当前顶点法向量的Y 分量
		float nz=FileUtil::myReadFloat(binaryData, binaryData_index);//读取当前顶点法向量的Z 分量
		Vector3f* vt=new Vector3f(nx,ny,nz); //创建存储当前顶点法向量的复合数对象
		normals.push_back(vt); //将法向量复合数对象指针存入相应列表
	}
	float s1 = FileUtil::myReadFloat(binaryData, binaryData_index); //读取第一个顶点的纹理S 坐标
	float s2 = FileUtil::myReadFloat(binaryData, binaryData_index); //读取第二个顶点的纹理S 坐标
	float s3 = FileUtil::myReadFloat(binaryData, binaryData_index); //读取第三个顶点的纹理S 坐标
	s = new Vector3f(s1, s2, s3); //创建存储三角形面三个顶点纹理S 坐标的复合数对象
	float t1 = FileUtil::myReadFloat(binaryData, binaryData_index); //读取第一个顶点的纹理T 坐标
	float t2 = FileUtil::myReadFloat(binaryData, binaryData_index); //读取第二个顶点的纹理T 坐标
	float t3 = FileUtil::myReadFloat(binaryData, binaryData_index); //读取第三个顶点的纹理T 坐标
	t=new Vector3f(t1,t2,t3); //创建存储三角形面三个顶点纹理T 坐标的复合数对象
	smoothingGroup=FileUtil::myReadByte(binaryData,binaryData_index);//读取三角形面所处平滑组编号
	groupIndex =FileUtil::myReadByte(binaryData, binaryData_index); //读取三角形面所处的组索引
}
int* MS3DTriangle::getIndexs(){ //获取三角形面三个顶点索引的方法
	return indexs; //返回指向三角形三个顶点索引值数组首地址的指针
}
Vector3f* MS3DTriangle::getS(){ //获取三角形面三个顶点纹理S 坐标的方法
	return s; //返回存储三角形三个顶点纹理S 坐标的复合数对象的指针
}
Vector3f* MS3DTriangle::getT(){ //获取三角形面三个顶点纹理T 坐标的方法
	return t; //返回存储三角形三个顶点纹理T 坐标的复合数对象的指针
}
int MS3DTriangle::getSmoothingGroup(){ //获取三角形面所处的平滑组编号的方法
	return smoothingGroup; //返回三角形面所处的平滑组编号
}
int MS3DTriangle::getGroupIndex(){ //获取三角形面所处的组索引的方法
	return groupIndex; //返回三角形面所处的组索引
}
MS3DTriangle::~MS3DTriangle()
{
	delete []indexs;
	delete s;
	delete t;
	vector<Vector3f*>::iterator iter=normals.begin();
	for(;iter!=normals.end();iter++)
	{
		Vector3f* vt=(*iter);
		delete vt;
	}
	normals.clear();
}
