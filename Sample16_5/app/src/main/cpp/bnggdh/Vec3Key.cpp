#include "Vec3Key.h"
Vec3Key::Vec3Key() {
    this->time = 0.0F;
    this->key = new Vector3f();
}
Vec3Key::Vec3Key(float time, Vector3f* v) {
    this->time = time;
	this->key = new Vector3f(v->getX(), v->getY(), v->getZ());
}
Vec3Key::~Vec3Key() {
	delete key;
}