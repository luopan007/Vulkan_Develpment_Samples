#include "PlanetConstant.h"
float PlanetConstant::perAngle=0; //小星体自转角
float PlanetConstant::rotateAngle=0; //小星体公转角
float PlanetConstant::groupNumber=15; //每组星体的数量
float PlanetConstant::oneGroupNumber=20; //星体组数
float PlanetConstant::lineWidth=8.0f; //土星光环宽度
float PlanetConstant::paiNumber=groupNumber*oneGroupNumber; //星体总数量
float PlanetConstant::distanceDai=8.162f; //内圈光环内侧与土星的距离
float PlanetConstant::distanceDaiBig=22.0f; //外圈光环内侧与土星的距离
float PlanetConstant::linWidthSpan=lineWidth/(oneGroupNumber-1); //每组内部各个小星体之间的距离步进
float PlanetConstant::angleSpan=360/groupNumber; //每组的角度跨度
