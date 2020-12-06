#include <string>
#include <stdlib.h>
#include <mylog.h>
#include "Normal.h"
#include "LoadUtil.h"
float DIFF=0.0000001f;
Normal::Normal(float nx, float ny, float nz)
{
    this->nx=nx;
    this->ny=ny;
    this->nz=nz;
}
bool Normal::exist(Normal *nIn,set<Normal*> sn)
{
    for(Normal* nTemp:sn)
    {
        if(abs(nTemp->nx-nIn->nx)<DIFF&&abs(nTemp->ny-nIn->ny)<DIFF&&abs(nTemp->nz-nIn->nz)<DIFF)
        {
            return true;
        }
    }
    return false;
}
float *Normal::getAverage(set<Normal*> sn)
{
    float *result =new float[3]();
    for(Normal* tempHsn:sn)
    {
        result[0]+= tempHsn->nx;
        result[1]+= tempHsn->ny;
        result[2]+= tempHsn->nz;
    }
    return LoadUtil::vectorNormal(result);
}
