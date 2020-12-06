#ifndef __CameraUtil_H__
#define __CameraUtil_H__
#define CAMERA_R 40
#define ANGLE_MIN -55.0f
#define ANGLE_MAX 55.0f
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
    static float camera9Para[9];
    static void calCamera();
    static void flushMainCameraToMatrix();
    static void flushMirrorCameraToMatrix();
};
#endif
