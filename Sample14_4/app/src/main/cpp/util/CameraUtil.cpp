#include "CameraUtil.h"
#include <math.h>
#include "MatrixState3D.h"
#include "HelpFunction.h"
float CameraUtil::camera9Para[9];
float CameraUtil::cx = 0;
float CameraUtil::cy = 30;
float CameraUtil::cz = 60;
float tx=0;
float ty=0;
float tz=0;
float CameraUtil::currSightDis=60;
float CameraUtil::angdegElevation=30;
float CameraUtil::angdegAzimuth=180;
void CameraUtil::calCamera()
{
    double angradElevation = toRadians(angdegElevation);
    double angradAzimuth = toRadians(angdegAzimuth);
    cx = (float) (tx - currSightDis * cos(angradElevation)	* sin(angradAzimuth));
    cy = (float) (ty + currSightDis * sin(angradElevation));
    cz = (float) (tz - currSightDis * cos(angradElevation) * cos(angradAzimuth));
    camera9Para[0] = cx;
    camera9Para[1] = cy;
    camera9Para[2] = cz;
    camera9Para[3] = 0;
    camera9Para[4] = 0;
    camera9Para[5] = 0;
    camera9Para[6] = 0;
    camera9Para[7] = 1;
    camera9Para[8] = 0;
}
void CameraUtil::flushCameraToMatrix()
{
    MatrixState3D::setCamera
    (
            camera9Para[0],camera9Para[1],camera9Para[2],
            camera9Para[3],camera9Para[4],camera9Para[5],
            camera9Para[6],camera9Para[7],camera9Para[8]
    );
}