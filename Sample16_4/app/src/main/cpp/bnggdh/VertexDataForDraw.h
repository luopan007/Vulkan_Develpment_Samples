#ifndef _VERTEXDATAFORDRAW_H
#define _VERTEXDATAFORDRAW_H
#include "Animation.h"
class VertexDataForDraw {
public:
	float* position_init; //指向初始顶点位置数据数组首地址的指针
	float* position_curr; //指向当前顶点位置数据数组首地址的指针
	float* texCoord; //指向纹理坐标数据数组首地址的指针
	short* indices; //指向索引数据数组首地址的指针
	int numsIndex; //索引数量
	float* weight; //指向权重数据数组首地址的指针
	int* vec_bone_indices; //指向绑定骨骼索引数据数组首地址的指针
	int numsVec; //顶点数量
	VertexDataForDraw(Animation* animation); //构造函数
	~VertexDataForDraw(); //析构函数
	void init_0(map<int, string> boneId_indices); //初始化骨骼索引与名称id 对应关系map 的方法
	void updateVertex_0(); //更新顶点数据的方法
private:
	Animation* mAnimation; //指向动画信息对象的指针
	map<int, string> boneId_indices; //表示骨骼索引与名称ID 对应关系的map
	Vector3f* initVec; //更新顶点坐标数据用到的辅助变量
	Vector3f* vec;
	Vector3f* tempL;
	Vector3f* temp;
};
#endif