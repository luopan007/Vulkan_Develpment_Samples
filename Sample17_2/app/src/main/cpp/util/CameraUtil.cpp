#include <math.h>
#include "CameraUtil.h"
#include "MatrixState3D.h"
#include "HelpFunction.h"

float CameraUtil::num=40;
float CameraUtil::r=sqrt(num)/4-1;
float CameraUtil::cAngle=0;
float CameraUtil::targetX=r;
float CameraUtil::targetY=0;
float CameraUtil::targetZ=r;
float CameraUtil::upX=0;
float CameraUtil::upY=1;
float CameraUtil::upZ=0;
float CameraUtil::ANGLE_MIN=-360;
float CameraUtil::ANGLE_MAX=360;
float CameraUtil::CameraX=r;
float CameraUtil::CameraY=6;
float CameraUtil::CameraZ=r;
void CameraUtil::calculateCamera(float angle) {
    CameraX = (float) (r*sin(toRadians(angle)))+targetX;
    CameraZ = (float) (r*cos(toRadians(angle)))+targetZ;
    MatrixState3D::setCamera(CameraX,CameraY,CameraZ,  targetX,targetY,targetZ,  upX,upY,upZ);
}

