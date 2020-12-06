#ifndef __CameraUtil_H__
#define __CameraUtil_H__
#define CAMERA_R 60
#define ANGLE_MIN -55.0f
#define ANGLE_MAX 55.0f
#define YANGLE_MIN 15.0f
#define YANGLE_MAX 85.0f
class CameraUtil
{
public:
    static float cx;
    static float cy;
    static float cz;
    static float tx;
    static float ty;
    static float tz;
    static float upx;
    static float upy;
    static float upz;
    static float m_cx;
    static float m_cy;
    static float m_cz;
    static float xAngle;
    static float yAngle;
    static float camera9Para[9];
    static void calCamera();
    static void calMainCamera();
    static void calMirrorCamera();
    static void flushCameraToMatrix();
};
#endif
