#include <math.h>
#include <vector>
#include "Planet.h"
#include "HelpFunction.h"
#include "PlanetConstant.h"
#include "mylog.h"

using namespace std;

Planet::Planet(int id,bool isBigDistance) {
    this->id=id;
    paiNumber=PlanetConstant::paiNumber;
    rotateAngle=PlanetConstant::rotateAngle;
    groupNumber=PlanetConstant::groupNumber;
    oneGroupNumber=PlanetConstant::oneGroupNumber;
    if(isBigDistance){
        distanceDai=PlanetConstant::distanceDai;
    }else{
        distanceDai=PlanetConstant::distanceDaiBig;
    }
    linWidthSpan=PlanetConstant::linWidthSpan;
    angleSpan=PlanetConstant::angleSpan;
    angleSpanAll=0.05f;
}
void Planet::initInstanceData() {
    for(int i=0;i<groupNumber;i++){
        float curAngle=i*angleSpan;//当前的角度
        for(int j=0;j<oneGroupNumber;j++){
            float translateDis=distanceDai+linWidthSpan*j;//当前平移距离
            float stoneAngle=curAngle+rotateAngle+angleSpan*random(0.0f,1.0f);//石头的旋转角度
            float translateX=translateDis*cos(toRadians(stoneAngle));//X方向的平移
            float translateZ=translateDis*sin(toRadians(stoneAngle));//Z方向的平移

            attributeQiu.push_back(translateDis);//将半径存入列表
            attributeQiu.push_back(stoneAngle);//将角度存入列表
            attributeQiu.push_back(translateX);//X位移
            attributeQiu.push_back(translateZ);//Z位移位移
        }
    }
}
void Planet::update(){
    for(int i=0;i<paiNumber;i++){
        if(attributeQiu[i*4+1]>=360){
            attributeQiu[i*4+1]=0;
        }
        attributeQiu[i*4+1]+=angleSpanAll;
        attributeQiu[i*4+2]=attributeQiu[i*4+0]*cos(toRadians(attributeQiu[i*4+1]));//X方向的平移
        attributeQiu[i*4+3]=attributeQiu[i*4+0]*sin(toRadians(attributeQiu[i*4+1]));//Z方向的平移
    }

}