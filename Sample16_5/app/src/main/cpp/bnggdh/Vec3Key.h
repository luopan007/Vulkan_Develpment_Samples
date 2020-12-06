#ifndef _VEC3KEY_H
#define _VEC3KEY_H
#include "Vector3f.h"
class Vec3Key {
public:
    float time;
    Vector3f *key;
    Vec3Key();
	~Vec3Key();
	/*
	*@param    time		当前平移关键帧的时间
	*@param    quat		指向当前平移动画对应的三维向量对象的指针
	*/
    Vec3Key(float time, Vector3f* v);
};
#endif
