#ifndef __CameraUtil_H__
#define __CameraUtil_H__
#define CAMERA_R 300
class CameraUtil
{
public:
    static float degree;
    static float yj;
    static float camera9Para[9];
    static void calCamera(float yjSpan,float cxSpan);
    static void flushCameraToMatrix();
};
#endif
