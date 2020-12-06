#ifndef __CameraUtil_H__
#define __CameraUtil_H__
#define CAMERA_R 5
class CameraUtil
{
public:
    static float tx;
    static float ty;
    static float tz;
    static float degree;
    static float yj;
    static float camera9Para[9];
    static void calCamera(float yjSpan,float cxSpan);
    static void flushCameraToMatrix();
private:
    static void calCamera();
};
#endif
