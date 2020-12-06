#ifndef _QUATKEY_H
#define _QUATKEY_H
#include "Quaternion.h"
class QuatKey {
public:
    float time;
    Quaternion *key;
    QuatKey();
	~QuatKey();
	/*
	*@param    time		当前旋转关键帧的时间
	*@param    quat		指向当前旋转动画对应的四元数对象的指针
	*/
    QuatKey(float time, Quaternion* quat);
};
#endif
