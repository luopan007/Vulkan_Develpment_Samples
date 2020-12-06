#include "CameraUtil.h"
#include <math.h>
#include "MatrixState3D.h"
#include "HelpFunction.h"
float CameraUtil::cAngle = 0;
float CameraUtil::camera9Para[9];
float CameraUtil::cx = 0;
float CameraUtil::cy = 30;
float CameraUtil::cz = 60;
float CameraUtil::tx = 0;
float CameraUtil::ty = 0;
float CameraUtil::tz = 0;
void CameraUtil::calCamera(float dx, float dy)
{
    cAngle += dx * 180.0f / 320.0f;
    cx = (float)sin(toRadians(cAngle))*CAMERA_R;
    cz = (float)cos(toRadians(cAngle))*CAMERA_R;
    cy += dy / 10.0f;
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
                    camera9Para[0], camera9Para[1], camera9Para[2],
                    camera9Para[3], camera9Para[4], camera9Para[5],
                    camera9Para[6], camera9Para[7], camera9Para[8]
            );
}