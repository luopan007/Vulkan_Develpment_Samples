/*
 * MS3DGroup.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DGroup.h"
#include <../util/FileUtil.h>
MS3DGroup::MS3DGroup(unsigned char* binaryData,int* binaryData_index){ //构造函数
	FileUtil::myReadByte(binaryData, binaryData_index);//读取该组在编辑器中的状态，本案例中无用
	FileUtil::myReadString(binaryData, binaryData_index,32); //读取组名称，本案例中无用
	count_ind =FileUtil::myReadUnsignedShort(binaryData, binaryData_index);//读取组内三角形数量
	indicies=new int[count_ind]; //初始化组内三角形索引数组
	for(int j=0; j<count_ind; j++){ //读取组内各个三角形的索引
		indicies[j]=FileUtil::myReadUnsignedShort(binaryData, binaryData_index);//读取一个三角形索引
	}
	materialIndex =FileUtil::myReadByte(binaryData, binaryData_index); //读取组对应的材质索引
}
int* MS3DGroup::getIndicies(){ //获取组内三角形索引的方法
	return indicies; //返回指向组内三角形索引数组首地址的指针
}
int MS3DGroup::getMaterialIndex(){ //获取组对应的材质索引的方法
	return materialIndex; //返回组对应的材质索引
}
int MS3DGroup::getCount_ind(){ //获取组内三角形数量的方法
	return count_ind; //返回组内三角形的数量
}
MS3DGroup::~MS3DGroup()
{
	delete indicies;
}
