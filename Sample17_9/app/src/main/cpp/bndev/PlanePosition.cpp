#include "PlanePosition.h"
#include "HelpFunction.h"
#include "PlaneConstant.h"

PlanePosition::PlanePosition(int id) {
    this->count=PlaneConstant::count;
    this->id=id;
    this->width=PlaneConstant::width;
    this->widSpan=PlaneConstant::widSpan;
    this->YMax=PlaneConstant::YMax;
    this->ZMax=PlaneConstant::ZMax;
    this->YSpan=PlaneConstant::YSpan;
    initInstanceData();
}
void PlanePosition::initInstanceData() {
    for(int i=0;i<count;i++){
        float xPos=-width/2+widSpan*i;
        float yPos=YMax*random(0.0f,1.0f)-YMax/2;
        float zPos=ZMax*random(0.0f,1.0f)-ZMax/2;
        curPosition.push_back(xPos);//x位置
        curPosition.push_back(yPos);//y位置
        curPosition.push_back(zPos);//z位置
        curPosition.push_back(1);//当前姿态（上升下降）
    }
}
void PlanePosition::update() {
    for(int i=0;i<count;i++){
        if(curPosition[i*4+1]>=YMax/2){
            curPosition[i*4+3]=-1;
        }
        if(curPosition[i*4+1]<=-YMax/2){
            curPosition[i*4+3]=1;
        }
        curPosition[i*4+1]=curPosition[i*4+3]*YSpan+curPosition[i*4+1];
    }
}