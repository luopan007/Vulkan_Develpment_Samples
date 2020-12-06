#include <math.h>
#include <sstream>
#include <../util/HelpFunction.h>
#include "Quaternion.h"
void Quaternion::angleToQuaternion(float x, float y, float z, Quaternion* q)
{
	float angle = x * 0.5F;
	float sin0 = (float)sin((double)angle);
	float cos0 = (float)cos((double)angle);
	angle = y * 0.5F;
	float sin1 = (float)sin((double)angle);
	float cos1 = (float)cos((double)angle);
	angle = z * 0.5F;
	float sin2 = (float)sin((double)angle);
	float cos2 = (float)cos((double)angle);
	q->x0 = sin0 * cos1 * cos2 - cos0 * sin1 * sin2;
	q->x1 = cos0 * sin1 * cos2 + sin0 * cos1 * sin2;
	q->x2 = cos0 * cos1 * sin2 - sin0 * sin1 * cos2;
	q->x3 = cos0 * cos1 * cos2 + sin0 * sin1 * sin2;
}
Quaternion::Quaternion(){
}
Quaternion::Quaternion(float x0, float x1, float x2)
{
    angleToQuaternion(toRadians(x0), toRadians(x1), toRadians(x2),this);
}
Quaternion::Quaternion(float x0, float x1, float x2, float x3) {
    this->x0 = x0;
    this->x1 = x1;
    this->x2 = x2;
    this->x3 = x3;
}
Quaternion::~Quaternion(){
}
void Quaternion::getVector4fRotate(Vector4f* v4)
{
	v4->setXYZW(x0, x1, x2, x3);
}
string Quaternion::toString() {
    return float2String(x0) + (string)" + " + float2String(x1) + "i + " + float2String(x2) + "j + " + float2String(x3) + "k";
}
