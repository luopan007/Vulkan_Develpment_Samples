#include <mylog.h>
#include "../util/HelpFunction.h"
#include "VertexDataForDraw.h"
VertexDataForDraw::VertexDataForDraw(Animation* animation) 
{
    this->mAnimation = animation;
    this->initVec = new Vector3f(0.0f, 0.0f, 0.0f);
    this->vec = new Vector3f[4]{Vector3f(0.0f, 0.0f, 0.0f)};
    this->tempL = new Vector3f(0.0f, 0.0f, 0.0f);
    this->temp = new Vector3f(0.0f, 0.0f, 0.0f);
}
void VertexDataForDraw::init_0(map<int, string> boneId_indices) {
    this->boneId_indices = boneId_indices;
}
void VertexDataForDraw::updateVertex_0() {
    for(int i = 0; i < this->numsVec; ++i) { //遍历每一个顶点
        this->initVec->setXYZ(this->position_init[i * 3 + 0], //初始化顶点位置
                              this->position_init[i * 3 + 1], this->position_init[i * 3 + 2]);
        for (int j = 0; j < 4; ++j) { //一个顶点最多有四个骨骼，故循环四次
            if (this->weight[i * 4 + j] != 0.0f) { //若权值不为0
                int k = this->vec_bone_indices[i * 4 + j]; //获取骨骼索引
                string id = (string) this->boneId_indices.at(k); //获取骨骼名称ID
                Mat4 *matrix; //实时变换矩阵
                Mat4 *absolute; //绝对矩阵
                if (this->mAnimation->mND_matrix.find(id) != this->mAnimation->mND_matrix.end()) {
                    matrix = this->mAnimation->mND_matrix.at(id); //获取骨骼的实时变换矩阵
                } else { LOGE("matrix == NULL"); }
                if (this->mAnimation->mND_absolute.find(id) !=
                    this->mAnimation->mND_absolute.end()) {
                    absolute = this->mAnimation->mND_absolute.at(id); //获取骨骼的绝对矩阵
                } else { LOGE("absolute == NULL"); }
                absolute->invTransformAndRotate(this->initVec, this->temp);//通过绝对矩阵对顶点进行逆变换
                matrix->transform(this->temp, &this->vec[j]); //通过实时变换矩阵对顶点继续进行变换得到新的位置
            } else { //若权值为0
                this->vec[j].setXYZ(0.0f, 0.0f, 0.0f); //权值为0 情况下顶点的此套坐标值不影响最后结果
            }
        }
        this->tempL->setXYZ(0.0f, 0.0f, 0.0f); //重置辅助变量
        this->tempL->interpolate_four(//根据权重混合四个向量，以获得四个骨骼影响下顶点的位置
                &this->vec[0], &this->vec[1], &this->vec[2], &this->vec[3],
                this->weight[i * 4 + 0], this->weight[i * 4 + 1], this->weight[i * 4 + 2],
                this->weight[i * 4 + 3]);
        this->position_curr[i * 3 + 0] = this->tempL->getX(); //设置新的顶点位置坐标X 分量
        this->position_curr[i * 3 + 1] = this->tempL->getY(); //设置新的顶点位置坐标Y 分量
        this->position_curr[i * 3 + 2] = this->tempL->getZ(); //设置新的顶点位置坐标Z 分量
    }}
VertexDataForDraw::~VertexDataForDraw()
{
	delete[] position_init;
	delete[] position_curr;
	delete[] texCoord;
	delete[] indices;
	delete[] weight;
	delete[] vec_bone_indices;
	delete initVec;
	delete[] vec;
	delete tempL;
	delete temp;
}