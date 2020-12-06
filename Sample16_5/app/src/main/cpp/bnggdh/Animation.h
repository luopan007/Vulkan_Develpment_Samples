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
	vector<string> boneVector;			
    map<string, Mat4*> mND_absolute;
    map<string, Mat4*> mND_relative;
    map<string, Mat4*> mND_matrix;
    map<string, string> mND_id;
    map<string, vector<Vec3Key*>> mTranslationKeys;
    map<string, vector<QuatKey*>> mRotationKeys;
    Animation();
	~Animation();
    void updateJoint(float time);
private:
	Vector4f *v4Helper= new Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	Vector4f *v4HelperLeft = new Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	Vector4f *v4HelperRight = new Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	Vector3f *v3Helper= new Vector3f(0.0f, 0.0f, 0.0f);
	Mat4* m4Helper = new Mat4();
	/*
	 * @param time		动画时间
	 * @param id		骨骼名称ID
	 * @param m			结果矩阵
	*/
    bool rotate(float time, string id, Mat4* m);
    bool translate(float time, string id, Mat4* m);
};
#endif