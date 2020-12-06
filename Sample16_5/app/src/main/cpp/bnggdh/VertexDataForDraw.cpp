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
VertexDataForDraw::~VertexDataForDraw()
{
	delete[] position_init;
	delete[] position_curr;
	delete[] normals;
	delete[] texCoord;
	delete[] indices;
	delete[] weight;
	delete[] vec_bone_indices;
	delete initVec;
	delete[] vec;
	delete tempL;
	delete temp;
}
void VertexDataForDraw::init_0(map<int, string> boneId_indices) {
    this->boneId_indices = boneId_indices;
}
void VertexDataForDraw::updateVertex_0() {
    for(int i = 0; i < this->numsVec; ++i) {
        this->initVec->setXYZ(this->position_init[i * 3 + 0], this->position_init[i * 3 + 1], this->position_init[i * 3 + 2]);
        for(int j = 0; j < 4; ++j) {
            if(this->weight[i * 4 + j] != 0.0f) {
                int k = this->vec_bone_indices[i * 4 + j];
                string id = (string)this->boneId_indices.at(k);
                Mat4* matrix;
                Mat4* absolute;
                if(this->mAnimation->mND_matrix.find(id)!=this->mAnimation->mND_matrix.end()){
                    matrix = this->mAnimation->mND_matrix.at(id);
                }
                else{
                    LOGE("matrix == NULL");
                }
                if(this->mAnimation->mND_absolute.find(id)!=this->mAnimation->mND_absolute.end()){
                    absolute = this->mAnimation->mND_absolute.at(id);
                }
                else{
                    LOGE("absolute == NULL");
                }
                absolute->invTransformAndRotate(this->initVec, this->temp);
                matrix->transform(this->temp, &this->vec[j]);
            } else {
                this->vec[j].setXYZ(0.0f, 0.0f, 0.0f);
            }
        }
        this->tempL->setXYZ(0.0f, 0.0f, 0.0f);
        this->tempL->interpolate_four(&this->vec[0], &this->vec[1], &this->vec[2], &this->vec[3], this->weight[i * 4 + 0], this->weight[i * 4 + 1], this->weight[i * 4 + 2], this->weight[i * 4 + 3]);
        this->position_curr[i * 3 + 0] = this->tempL->getX();
        this->position_curr[i * 3 + 1] = this->tempL->getY();
        this->position_curr[i * 3 + 2] = this->tempL->getZ();
    }
}
