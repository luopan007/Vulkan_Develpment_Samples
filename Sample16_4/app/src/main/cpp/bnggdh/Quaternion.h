#ifndef _QUATERNION_H
#define _QUATERNION_H
#include "Vector4f.h"
#include <string>
using  namespace std;
class Quaternion 
{
private:
	static void angleToQuaternion(float x, float y, float z, Quaternion* q);
public:
    float x0;
    float x1;
	float x2;
    float x3;
    Quaternion();
    ~Quaternion();
	/**
	* @param x0 角度值(非弧度制)
	* @param x1 角度值(非弧度制)
	* @param x2 角度值(非弧度制)
	*/
	Quaternion(float x0, float x1, float x2);
	Quaternion(float x0, float x1, float x2, float x3);
	string toString();
	void getVector4fRotate(Vector4f* v4);
};
#endif
