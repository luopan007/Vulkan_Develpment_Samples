#ifndef _VERTEXDATAFORDRAW_H
#define _VERTEXDATAFORDRAW_H
#include "Animation.h"
class VertexDataForDraw {
public:
    float* position_init;
    float* position_curr;
    float* normals;
    float* texCoord;
    short* indices;
    int numsIndex;
    float* weight;
    int* vec_bone_indices;
    int numsVec;
    VertexDataForDraw(Animation* animation);
	~VertexDataForDraw();
    void init_0(map<int, string> boneId_indices);
    void updateVertex_0();
private:
    Animation* mAnimation;
    map<int, string> boneId_indices;
    Vector3f* initVec;
    Vector3f* vec;
    Vector3f* tempL;
    Vector3f* temp;
};
#endif
