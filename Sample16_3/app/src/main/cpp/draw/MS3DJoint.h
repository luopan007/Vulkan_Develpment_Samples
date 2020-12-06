/*
 * MS3DJoint.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#ifndef MS3DJOINT_H_
#define MS3DJOINT_H_
#include "../element/Vector3f.h"
#include "../element/Mat4.h"
#include "MS3DKeyFrameRotate.h"
#include "MS3DKeyFramePosition.h"
#include <string>
#include <vector>
using namespace std;
class MS3DJoint{
private:
	Mat4* tmatrix; //指向当前绝对矩阵(实时变换矩阵)对象的指针
	vector<MS3DKeyFrameRotate*> rotates; //存储关键帧旋转数据对象指针的列表
	vector<MS3DKeyFramePosition*> positions; //存储关键帧平移数据对象指针的列表
	int count_rot; //关节的旋转关键帧数量
	int count_pos; //关节的平移关键帧数量
	Vector3f* tranV3Helper = new Vector3f(); //用于对平移关键帧进行插值计算的辅助变量
	Vector4f* tranV4Helper = new Vector4f(); //用于对旋转关键帧进行插值计算的辅助变量
	Mat4* mHelper = new Mat4(); //用于矩阵计算的辅助变量
public:
	string name; //关节名称
	string parentName; //父关节名称
	bool ifparent; //是否存在父关节的标志
	Vector3f* trotate; //指向用于存储初始旋转值的复合数对象的指针
	Vector3f* tposition; //指向用于存储初始位置值的复合数对象的指针
	Mat4* relative; //指向相对矩阵(子关节在父关节坐标系中的变换矩阵)对象的指针
	Mat4* absolute; //指向初始绝对矩阵(子关节在世界坐标系中的初始变换矩阵)对象的指针
	MS3DJoint* parent; //指向父关节对象的指针
	MS3DJoint(unsigned char* binaryData,int* binaryData_index);//构造函数
	~MS3DJoint(); //析构函数
	void update(float ttime); //更新关节数据的方法
	Mat4* ttrotate(float time); //根据当前播放时间和关键帧数据进行旋转插值计算的方法
	Vector3f* ttposition(float time); //根据当前播放时间和关键帧数据进行平移插值计算的方法
	Mat4* getMatrix(); //获取当前绝对矩阵的方法
	Mat4* getAbsolute(); //获取初始绝对矩阵的方法
};
#endif
