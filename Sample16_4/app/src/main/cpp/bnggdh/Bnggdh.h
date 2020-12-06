#ifndef _BNGGDH_H
#define _BNGGDH_H
#include "VertexDataForDraw.h"
#include "Animation.h"
class Bnggdh {
private:
	VertexDataForDraw* vdfd;
    Animation* mAnimation;
    float maxKeytime;
    float* normals=NULL;
    float* vNormal = new float[3];
    string sourceName;
public:
    float* getCurrentNormal();
    int getVerNums();
    int getIndexNums();
    float* getMatrix(string id);
    float getMaxKeytime();
    float* getTextures();
    short* getIndices();
    float* getPosition();
    void updata(float time);
    void init();
    Bnggdh(string sourceName);
    ~Bnggdh();
};
#endif
