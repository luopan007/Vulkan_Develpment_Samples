#include <string>
#include <stdlib.h>
#include <mylog.h>
#include "Normal.h"
#include "LoadUtil.h"

float DIFF=0.0000001f; //判断两个向量是否相同的阈值
Normal::Normal(float nx, float ny, float nz){ //构造函数
    this->nx=nx;this->ny=ny;this->nz=nz; //接收当前向量的XYZ 方向分量值并保存
}
bool Normal:: exist (Normal *nIn,set<Normal*> sn){ //判断一个向量是否在指定集合中的方法
    for(Normal* nTemp: sn){ //遍历指定集合中的所有向量
        if(abs(nTemp->nx-nIn->nx)<DIFF&&abs(nTemp->ny-nIn->ny)<DIFF&&abs(nTemp->nz-nIn->nz)<DIFF) //判断两个向量是否相等
        {return true;}} //若相等则返回true
    return false; //若集合中没有相同向量则返回false
}


float *Normal::getAverage(set<Normal*> sn){ //求向量平均值的方法
    float *result =new float[3](); //存放结果向量X、Y、Z 分量值的数组
    for(Normal* tempHsn:sn){ //遍历集合中的所有向量
        result[0]+= tempHsn->nx; //叠加集合中所有向量的X 分量
        result[1]+= tempHsn->ny; //叠加集合中所有向量的Y 分量
        result[2]+= tempHsn->nz; //叠加集合中所有向量的Z 分量
    }
    return LoadUtil::vectorNormal(result); //将求和后的向量规格化以得到平均向量
}


