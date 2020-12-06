#include "CameraUtil.h"
#include <math.h>
#include "MatrixState3D.h"
#include "Constant.h"
float CameraUtil::camera9Para[9];
float CameraUtil::cx = 0;
float CameraUtil::cy = 0;
float CameraUtil::cz = 0;
float CameraUtil::tx = 0;
float CameraUtil::ty = 0;
float CameraUtil::tz = 0;
float CameraUtil::upx = 0;
float CameraUtil::upy = 1;
float CameraUtil::upz = 0;
float CameraUtil::m_cx = 0;
float CameraUtil::m_cy = 0;
float CameraUtil::m_cz = 0;
float CameraUtil::xAngle = 0;
float CameraUtil::yAngle = 0;
float max(float a,float b)
{
    return a>b?a:b;
}
float min(float a,float b)
{
    return a>b?b:a;
}
void CameraUtil::calCamera()
{
    xAngle = max(xAngle, ANGLE_MIN);
    xAngle = min(xAngle, ANGLE_MAX);
    yAngle = max(yAngle, YANGLE_MIN);
    yAngle = min(yAngle, YANGLE_MAX);
    cx = (float)(CAMERA_R*cos(yAngle*PI / 180)*sin(xAngle*PI / 180));
    cy = (float)(CAMERA_R*sin(yAngle*PI / 180));
    cz = (float)(CAMERA_R*cos(yAngle*PI / 180)*cos(xAngle*PI / 180));
    m_cx = cx;
    m_cy = 2 * ty -cy;
    m_cz = cz;
}
void CameraUtil::calMainCamera()
{
    camera9Para[0] = cx;
    camera9Para[1] = cy;
    camera9Para[2] = cz;
    camera9Para[3] = tx;
    camera9Para[4] = ty;
    camera9Para[5] = tz;
    camera9Para[6] = upx;
    camera9Para[7] = upy;
    camera9Para[8] = upz;
}
void CameraUtil::calMirrorCamera()
{
    camera9Para[0] = m_cx;
    camera9Para[1] = m_cy;
    camera9Para[2] = m_cz;
    camera9Para[3] = tx;
    camera9Para[4] = ty;
    camera9Para[5] = tz;
    camera9Para[6] = upx;
    camera9Para[7] = upy;
    camera9Para[8] = upz;
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