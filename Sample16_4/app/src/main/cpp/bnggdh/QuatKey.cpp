#include "QuatKey.h"
QuatKey::QuatKey() {
	this->time = 0.0F;
	this->key = new Quaternion();
}
QuatKey::QuatKey(float time, Quaternion* quat) {
    this->time = time;
    this->key = new Quaternion(quat->x0, quat->x1, quat->x2, quat->x3);
}
QuatKey::~QuatKey() {
	delete key;
}
