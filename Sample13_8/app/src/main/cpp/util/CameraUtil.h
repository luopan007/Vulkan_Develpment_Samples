#ifndef __CameraUtil_H__
#define __CameraUtil_H__
#define CAMERA_R 60
class CameraUtil
{
public:
    static float cx;
    static float cy;
    static float cz;
    static float tx;
    static float ty;
    static float tz;
    static float cAngle;
    static float camera9Para[9];
    static void calCamera(float yjSpan, float cxSpan);
    static void flushCameraToMatrix();
};
#endif
