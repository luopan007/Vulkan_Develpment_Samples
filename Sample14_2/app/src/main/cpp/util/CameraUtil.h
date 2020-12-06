#ifndef __CameraUtil_H__
#define __CameraUtil_H__
#define CAMERA_R 60
class CameraUtil
{
public:
    static float degree;//摄像机朝向角度
    static float yj;//摄像机仰角

    static float camera9Para[9];
    static void calCamera(float yjSpan,float cxSpan);
    static void flushCameraToMatrix();
};
#endif
