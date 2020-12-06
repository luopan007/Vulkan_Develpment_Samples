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
		}else {
			int size = rotateQ.size();
			int index;
			for (index = 0; index < size && rotateQ[index]->time < time; ++index) {;}
			if (index == 0) {
				rotateQ[0]->key->getVector4fRotate(v4Helper);
				m->genRotateFromQuaternion(v4Helper);
			}else if (index == size) {
				rotateQ[size - 1]->key->getVector4fRotate(v4Helper);
				m->genRotateFromQuaternion(v4Helper);
			}else {
				QuatKey *left = rotateQ[index - 1];
				QuatKey *right = rotateQ[index];
				v4Helper->setXYZW(0, 0, 0, 1);
				left->key->getVector4fRotate(v4HelperLeft);
				right->key->getVector4fRotate(v4HelperRight);
				v4Helper->interpolate(v4HelperLeft,
									  v4HelperRight,(time-left->time)/(right->time- left->time));
				m->genRotateFromQuaternion(v4Helper);
			}return true;
		}}}
bool Animation::translate(float time, string id, Mat4* m) { //获取骨骼平移数据的方法
	if (mTranslationKeys.find(id) == mTranslationKeys.end()){ //若没有此骨骼的平移动画信息
		v3Helper->setXYZ(0.0f, 0.0f, 0.0f); //设置辅助三分量复合数对象的值（表示没有平移变换）
		m->setTranslation(v3Helper); //将平移变换信息记录进矩阵
		return false; //返回
	}else { //若有此骨骼的平移动画信息
		vector<Vec3Key*> vec3Key =this->mTranslationKeys.at(id);//获取骨骼的平移数据列表
		if (vec3Key[0]->time > time||vec3Key[0]->time < 0.0f) {//若第1 个关键帧的时间不在正常范围内
			v3Helper->setXYZ(0.0f, 0.0f, 0.0f); //设置辅助三分量复合数对象的值（表示没有平移变换）
			m->setTranslation(v3Helper); //将平移变换信息记录进矩阵
			return false; //返回
		}else {
			int size = vec3Key.size(); //获取平移关键帧的数量
			int index; //关键帧索引
			for (index = 0; //计算关键帧时间大于当前时间time 的第1 个关键帧索引
				 index < size && vec3Key[index]->time < time; ++index) {;}
			Vector3f* v; //辅助临时指针
			if (index == 0) { //若关键帧索引为0
				v = (vec3Key[0])->key; //获取第一帧的数据对象
			}else if (index == size) { //若关键帧索引等于关键帧数量
				v = (vec3Key[size - 1])->key; //获取最后一帧的数据对象
			}else { //若关键帧索引既不是起始帧也不是结束帧
				Vec3Key* left = vec3Key[index - 1]; //上一关键帧的平移数据
				Vec3Key* right = vec3Key[index]; //此关键帧的平移数据
				v = v3Helper;
				v->interpolate(left->key, //插值计算当前平移数据
							   right->key, (time - left->time) / (right->time - left->time));
			}
			m->setTranslation(v); //将旋转变换数据记录进矩阵
			return true;
		}}}
void Animation::updateJoint(float time){ //更新骨骼数据的方法
	for (string id : boneVector){ //遍历所有的骨骼
		bool flag = (rotate(time, id, m4Helper) | translate(time, id, m4Helper));//获取旋转和平移变换数据
		if (mND_id.at(id) != "") { //若有父骨骼
			string id_father = mND_id.at(id); //取出父骨骼名称ID
			if (!flag){ //若骨骼自身没有旋转/平移
				m4Helper->mul //骨骼当前变换矩阵为父骨骼矩阵乘以自身相对矩阵
						(this->mND_matrix.at(id_father), this->mND_relative.at(id));
			}else{ //若骨骼自身有旋转/平移
				m4Helper->mul //骨骼当前变换矩阵为父骨骼矩阵乘以记录了自身旋转平移信息的矩阵
						(this->mND_matrix.at(id_father), m4Helper);
			}
		}else if (!flag){ //若没有父骨骼且自身没有旋转平移
			m4Helper->mul(m4Helper,this->mND_absolute.at(id));//乘以绝对矩阵
		}
		if (mND_matrix[id] == nullptr){ //若当前骨骼没有对应变换矩阵对象
			mND_matrix[id] = new Mat4(); //创建当前骨骼对应的变换矩阵对象
		}
		mND_matrix[id]->copyFrom(m4Helper); //拷贝当前骨骼对应变换矩阵的元素
	}}