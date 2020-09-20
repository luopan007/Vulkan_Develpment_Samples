#ifndef SAMPLE5_1_CAMERAUTIL_H
#define SAMPLE5_1_CAMERAUTIL_H
#define CAMERA_R 600
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
