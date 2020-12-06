#include "Animation.h"
Animation::Animation() {
}
Animation::~Animation() {
	map<string, Mat4*>::iterator iter;
	for (iter = mND_absolute.begin(); iter != mND_absolute.end();)
	{
		delete iter->second;
		mND_absolute.erase(iter++);
	}
	mND_absolute.clear();
	for (iter = mND_relative.begin(); iter != mND_relative.end();)
	{
		delete iter->second;
		mND_relative.erase(iter++);
	}
	mND_relative.clear();
	for (iter = mND_matrix.begin(); iter != mND_matrix.end();)
	{
		delete iter->second;
		mND_matrix.erase(iter++);
	}
	mND_matrix.clear();
	for (map<string, string>::iterator iter = mND_id.begin(); iter != mND_id.end();)
	{
		mND_id.erase(iter++);
	}
	mND_id.clear();
	for (map<string, vector<Vec3Key*>>::iterator iter = mTranslationKeys.begin();iter != mTranslationKeys.end();)
	{
		for (Vec3Key* v : iter->second)
		{
			delete v;
		}
		mTranslationKeys.erase(iter++);
	}
	mTranslationKeys.clear();
	for (map<string, vector<QuatKey*>>::iterator iter = mRotationKeys.begin(); iter != mRotationKeys.end();)
	{
		for (QuatKey* q : iter->second)
		{
			delete q;
		}
		mRotationKeys.erase(iter++);
	}
	mRotationKeys.clear();
	delete v4Helper;
	delete v3Helper;
	delete m4Helper;
	delete v4HelperLeft;
	delete v4HelperRight;
}
bool Animation::rotate(float time, string id, Mat4* m) {
	if (mRotationKeys.find(id) == mRotationKeys.end()) {
		v4Helper->setXYZW(0, 0, 0, 1);
		m->genRotateFromQuaternion(v4Helper);
		return false;
	}
	else {
		vector<QuatKey*> rotateQ = this->mRotationKeys.at(id);
		if (rotateQ[0]->time > time||rotateQ[0]->time < 0.0f) {
			v4Helper->setXYZW(0, 0, 0, 1);
			m->genRotateFromQuaternion(v4Helper);
			return false;
		}
		else {
			int size = rotateQ.size();
			int index;
			for (index = 0; index < size && rotateQ[index]->time < time; ++index) {
				;
			}
			if (index == 0) {
				rotateQ[0]->key->getVector4fRotate(v4Helper);
				m->genRotateFromQuaternion(v4Helper);
			}
			else if (index == size) {
				rotateQ[size - 1]->key->getVector4fRotate(v4Helper);
				m->genRotateFromQuaternion(v4Helper);
			}
			else {
				QuatKey *left = rotateQ[index - 1];
				QuatKey *right = rotateQ[index];
				v4Helper->setXYZW(0, 0, 0, 1);
				left->key->getVector4fRotate(v4HelperLeft);
				right->key->getVector4fRotate(v4HelperRight);
				v4Helper->interpolate(v4HelperLeft, v4HelperRight, (time - left->time) / (right->time - left->time));
				m->genRotateFromQuaternion(v4Helper);
			}
			return true;
		}
	}
}
bool Animation::translate(float time, string id, Mat4* m) {
	if (mTranslationKeys.find(id) == mTranslationKeys.end())
	{
		v3Helper->setXYZ(0.0f, 0.0f, 0.0f);
		m->setTranslation(v3Helper);
		return false;
	}
	else {
		vector<Vec3Key*> vec3Key = this->mTranslationKeys.at(id);
		if (vec3Key[0]->time > time||vec3Key[0]->time < 0.0f) {
			v3Helper->setXYZ(0.0f, 0.0f, 0.0f);
			m->setTranslation(v3Helper);
			return false;
		}
		else {
			int size = vec3Key.size();
			int index;
			for (index = 0; index < size && vec3Key[index]->time < time; ++index) {
				;
			}
			Vector3f* v;
			if (index == 0) {
				v = (vec3Key[0])->key;
			}
			else if (index == size) {
				v = (vec3Key[size - 1])->key;
			}
			else {
				Vec3Key* left = vec3Key[index - 1];
				Vec3Key* right = vec3Key[index];
				v = v3Helper;
				v->interpolate(left->key, right->key, (time - left->time) / (right->time - left->time));
			}
			m->setTranslation(v);
			return true;
		}
	}
}
void Animation::updateJoint(float time)
{
	for (string id : boneVector)
	{
		bool flag = (rotate(time, id, m4Helper) | translate(time, id, m4Helper));
		if (mND_id.at(id) != "") {
			string id_father = mND_id.at(id);
			if (!flag)
			{
				m4Helper->mul(this->mND_matrix.at(id_father), this->mND_relative.at(id));
			}
			else
			{
				m4Helper->mul(this->mND_matrix.at(id_father), m4Helper);
			}
		}
		else if (!flag)
		{
			m4Helper->mul(m4Helper, this->mND_absolute.at(id));
		}
		if (mND_matrix[id] == nullptr)
		{
			mND_matrix[id] = new Mat4();
		}
		mND_matrix[id]->copyFrom(m4Helper);
	}
}
