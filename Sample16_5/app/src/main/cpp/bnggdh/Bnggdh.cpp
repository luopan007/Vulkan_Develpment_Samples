#include <../util/HelpFunction.h>
#include "Bnggdh.h"
#include "CalculateNormal.h"
#include "MyInputStream.h"
Bnggdh::Bnggdh(string sourceName)
{
    this->sourceName=sourceName;
    init();
}
float* Bnggdh::getCurrentNormal() 
{
    if(this->normals == NULL) 
	{
        this->normals = new float[this->getVerNums()*3]();
    }
    CalculateNormal::calNormal(this->getPosition(), this->getIndices(),this->getIndexNums(),this->getVerNums(), this->normals, this->vNormal);
    return this->normals;
}
int Bnggdh::getVerNums() {
    return this->vdfd->numsVec;
}
int Bnggdh::getIndexNums() {
    return this->vdfd->numsIndex;
}
float* Bnggdh::getMatrix(string id) {
    return this->mAnimation->mND_matrix.at(id)->matrix;
}
float Bnggdh::getMaxKeytime() {
    return this->maxKeytime;
}
float* Bnggdh::getTextures() {
    return this->vdfd->texCoord;
}
short* Bnggdh::getIndices() {
    return this->vdfd->indices;
}
float* Bnggdh::getPosition() {
    return this->vdfd->position_curr;
}
void Bnggdh::updata(float time) {
    this->mAnimation->updateJoint(time);
    this->vdfd->updateVertex_0();
}
void Bnggdh::init() {
    this->mAnimation = new Animation();
    this->vdfd = new VertexDataForDraw(this->mAnimation);
    MyInputStream* mis = new MyInputStream(this->sourceName);
    /*================================================bnggdh第一部分顶点数据=坐标量（顶点数*3）+坐标XYZ数据begin==============================================*/
    this->vdfd->numsVec = mis->myReadInt()/3;
    this->vdfd->position_init = mis->myReadFloats(this->vdfd->numsVec*3);
    this->vdfd->position_curr=new float[this->vdfd->numsVec*3];
    for(int i=0;i< this->vdfd->numsVec*3;i++)
    {
        vdfd->position_curr[i]=vdfd->position_init[i];
    }
    /*===================================================bnggdh第一部分纹理坐标数据=坐标量（顶点数*3）+坐标XYZ数据end=================================================*/
    /*================================================bnggdh第二部分顶点数据=纹理坐标量（顶点数*2）+纹理坐标ST数据begin================================================*/
    this->vdfd->texCoord = mis->myReadFloats(mis->myReadInt());
    /*================================================bnggdh第二部分顶点数据=纹理坐标量（顶点数*2）+纹理坐标ST数据end==================================================*/
    /*=============================================bnggdh第三部分索引数据=索引数据量（面数*3）+索引数据（顶点索引）begin===============================================*/
    int length=mis->myReadInt();
    this->vdfd->numsIndex=length;
    this->vdfd->indices=new short[length];
    int* indices0 = mis->myReadInts(length);
    for(int i = 0; i < length; ++i) {
        this->vdfd->indices[i] = (short)indices0[i];
    }
    /*=============================================bnggdh第三部分索引数据=索引数据量（面数*3）+索引数据（顶点索引）end===============================================*/
    /*=============================================bnggdh第四部分权重信息=权重数据量（顶点数量*4）+权重数据begin=====================================================*/
    this->vdfd->weight = mis->myReadFloats(mis->myReadInt());
    /*=============================================bnggdh第四部分权重信息=权重数据量（顶点数量*4）+权重数据end=======================================================*/
    /*===============================bnggdh第五部分顶点与骨骼绑定信息=模型中绑定骨骼索引的数量（顶点数量*4）+绑定骨骼索引数据begin===================================*/
    this->vdfd->vec_bone_indices = mis->myReadInts(mis->myReadInt());
    /*===============================bnggdh第五部分顶点与骨骼绑定信息=模型中绑定骨骼索引的数量（顶点数量*4）+绑定骨骼索引数据end=====================================*/
    /*==================================bnggdh第六部分骨骼索引信息=骨骼数量+《骨骼索引，骨骼名称ID<名称长度，名称字符串>》begin======================================*/
    map<int,string>index_boneIDm;
    int boneNums = mis->myReadInt();
    int boneIndex;
    string boneID;
    for(int i = 0; i < boneNums; ++i) {
        boneIndex = mis->myReadInt();
        boneID = mis->myReadString();
        index_boneIDm.insert(map<int,string>::value_type(boneIndex, boneID));
    }
    /*===================================bnggdh第六部分骨骼索引信息=骨骼数量+《骨骼索引，骨骼名称ID<名称长度，名称字符串>》end=======================================*/
    /*================bnggdh第七部分各个骨骼的绝对矩阵信息=骨骼数量+《骨骼名称ID<名称长度，名称字符串>，绝对矩阵数据<矩阵元素数量，矩阵数据>》begin===================*/
	boneNums = mis->myReadInt();
	for (int i = 0; i < boneNums; ++i) {
		boneID = mis->myReadString();
		float* absoluteMat4 = mis->myReadFloats(mis->myReadInt());
		Mat4* id = new Mat4();
		id->matrix = absoluteMat4;
		this->mAnimation->boneVector.push_back(boneID);
		this->mAnimation->mND_absolute.insert(map<string, Mat4*>::value_type(boneID, id));
	}
    /*=================bnggdh第七部分各个骨骼的绝对矩阵信息=骨骼数量+《骨骼名称ID<名称长度，名称字符串>，绝对矩阵数据<矩阵元素数量(16)，矩阵数据>》end====================*/
    /*bnggdh第八部分相对矩阵和父骨骼信息=骨骼数量+《骨骼名称ID<名称长度，名称字符串>，父骨骼名称ID<名称长度，名称字符串>，相对矩阵数据<矩阵元素数量(16)，矩阵数据>》begin=*/
    boneNums = mis->myReadInt();
    string father_boneID;
    for(int i = 0; i < boneNums; ++i) {
        boneID = mis->myReadString();
        father_boneID = mis->myReadString();
        float* relative = mis->myReadFloats(mis->myReadInt());
        Mat4* relM = new Mat4();
        relM->matrix = relative;
        this->mAnimation->mND_id.insert(map<string,string>::value_type(boneID, father_boneID));
        this->mAnimation->mND_relative.insert(map<string,Mat4*>::value_type(boneID, relM));
    }
    /*=bnggdh第八部分相对矩阵和父骨骼信息=骨骼数量+《骨骼名称ID<名称长度，名称字符串>，父骨骼名称ID<名称长度，名称字符串>，相对矩阵数据<矩阵元素数量(16)，矩阵数据>》end=*/
    /*bnggdh第九部分骨骼动画信息=骨骼的动画组数量+《骨骼名称ID<名称长度，名称字符串>,骨骼关键帧数量int，关键帧float，缩放数组长度，缩放数组，旋转数组长度，旋转数组，平移数组长度，平移数组》+骨骼动画关键帧的最大值begin*/
    int aniGroNums = mis->myReadInt();
    int boneKeyNums;
    for(int i = 0; i < aniGroNums; ++i) {
        boneID = mis->myReadString();
        boneKeyNums = mis->myReadInt();
        vector<Vec3Key*> tranKeyFragV;
        vector<QuatKey*> quatKeyFragV;
        for(int j = 0; j < boneKeyNums; ++j) {
            float time = mis->myReadFloat();
            mis->myReadFloats(mis->myReadInt());
            float* rotate = mis->myReadFloats(mis->myReadInt());
            float* translate = mis->myReadFloats(mis->myReadInt());
            Quaternion* quat = new Quaternion(rotate[0], rotate[1], rotate[2], rotate[3]);
            QuatKey* qk = new QuatKey(time, quat);
            quatKeyFragV.push_back(qk);
            Vector3f* v = new Vector3f(translate[0], translate[1], translate[2]);
            Vec3Key* vk = new Vec3Key(time, v);
            tranKeyFragV.push_back(vk);
        }
        this->mAnimation->mRotationKeys.insert(map<string,vector<QuatKey*>>::value_type(boneID, quatKeyFragV));
        this->mAnimation->mTranslationKeys.insert(map<string,vector<Vec3Key*>>::value_type(boneID, tranKeyFragV));
    }
    this->maxKeytime = mis->myReadFloat();
    /*bnggdh第九部分骨骼动画信息=骨骼的动画组数量+《骨骼名称ID<名称长度，名称字符串>,骨骼关键帧数量int，关键帧float，缩放数组长度，缩放数组，旋转数组长度，旋转数组，平移数组长度，平移数组》+骨骼动画关键帧的最大值end*/
    this->vdfd->init_0(index_boneIDm);
    delete mis;
}
Bnggdh::~Bnggdh() {
	delete vdfd;
	delete mAnimation;
	delete[] normals;
	delete[] vNormal;
}