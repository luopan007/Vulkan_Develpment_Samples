#ifndef __CameraUtil_H__
#define __CameraUtil_H__
#define CAMERA_R 60
class CameraUtil
{
public:
    static float cx;
    static float cy;
    static float cz;
    static float cAngle;
    static float currSightDis;
    static float angdegElevation;
    static float angdegAzimuth;
    static float camera9Para[9];
    static void calCamera();
    static void flushCameraToMatrix();
};
#endif
