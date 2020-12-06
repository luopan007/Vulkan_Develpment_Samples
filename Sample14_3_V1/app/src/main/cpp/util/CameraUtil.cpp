#include "CameraUtil.h"
#include <math.h>
#include <MyVulkanManager.h>
#include "MatrixState3D.h"
#include "HelpFunction.h"
#include "LightManager.h"
float CameraUtil::degree=0;
float CameraUtil::yj=0;
float CameraUtil::camera9Para[9];
void CameraUtil::calCamera()
{
    float cx=(float) sin(toRadians(MyVulkanManager::lAngle))*CAMERA_R;
    float cz=(float) cos(toRadians(MyVulkanManager::lAngle))*CAMERA_R;
    LightManager::setLightPosition(cx,50,cz);
    camera9Para[0]=cx;
    camera9Para[1]=10;
    camera9Para[2]=cz;
    camera9Para[3]=0;
    camera9Para[4]=0;
    camera9Para[5]=0;
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