#ifndef _ANIMATION_H
#define _ANIMATION_H
#include <map>
#include <vector>
#include "Mat4.h"
#include "Vec3Key.h"
#include "QuatKey.h"
using namespace std;
class Animation{
public:
	vector<string> boneVector; //存放骨骼名称的列表
	map<string, Mat4*> mND_absolute; //存放骨骼对应绝对矩阵的map
	map<string, Mat4*> mND_relative; //存放骨骼对应相对矩阵的map
	map<string, Mat4*> mND_matrix; //存放骨骼对应变换矩阵的map
	map<string, string> mND_id; //存放骨骼，父骨骼对应关系的map
	map<string, vector<Vec3Key*>> mTranslationKeys; //存放骨骼平移动画信息的map
	map<string, vector<QuatKey*>> mRotationKeys; //存放骨骼旋转动画信息的map
	Animation(); //构造函数
	~Animation(); //析构函数
	void updateJoint(float time); //更新骨骼数据的方法
private:
	Vector4f *v4Helper= new Vector4f(0.0f, 0.0f, 0.0f, 1.0f); //辅助变量
	Vector4f *v4HelperLeft = new Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	Vector4f *v4HelperRight = new Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	Vector3f *v3Helper= new Vector3f(0.0f, 0.0f, 0.0f);
	Mat4* m4Helper = new Mat4();
	bool rotate(float time, string id, Mat4* m); //获取骨骼旋转数据的方法
	bool translate(float time, string id, Mat4* m); //获取骨骼平移数据的方法
};
#endif