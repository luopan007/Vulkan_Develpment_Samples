#include "CameraUtil.h"
#include <math.h>
#include <mylog.h>
#include "MatrixState3D.h"
float CameraUtil::degree=0;
float CameraUtil::yj=0;
float CameraUtil::camera9Para[9];
void CameraUtil::calCamera(float yjSpan,float cxSpan)
{
    yj=yj+yjSpan;
    if(yj>90)
    {
        yj=90;
    }
    if(yj<10)
    {
        yj=10;
    }
    degree=degree+cxSpan;
    if(degree>=360)
    {
        degree=degree-360;
    }
    else if(degree<=0)
    {
        degree=degree+360;
    }
    float cy=sin(yj*3.1415926535898/180)*CAMERA_R;
    float cxz=cos(yj*3.1415926535898/180)*CAMERA_R;
    float cx=sin(degree*3.1415926535898/180)*cxz;
    float cz=cos(degree*3.1415926535898/180)*cxz;
    float upY=cos(yj*3.1415926535898/180);
    float upXZ=sin(yj*3.1415926535898/180);
    float upX=-upXZ*sin(degree*3.1415926535898/180);
    float upZ=-upXZ*cos(degree*3.1415926535898/180);
    camera9Para[0]=cx;
    camera9Para[1]=cy;
    camera9Para[2]=cz;
    camera9Para[3]=0;
    camera9Para[4]=0;
    camera9Para[5]=0;
    camera9Para[6]=upX;
    camera9Para[7]=upY;
    camera9Para[8]=upZ;
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