#ifndef __CameraUtil_H__
#define __CameraUtil_H__
#include <vector>
using namespace std;
#define CAMERA_R 20

class CameraUtil
{
public:

    static float tx;
    static float ty;
    static float tz;
    static float upx;
    static float upy;
    static float upz;
    static float cx;
    static float cy;
    static float cz;
    static float degree;
    static float yj;
    static float direction;
    static float camera9Para[9];
    static void calCamera();
    static void init3DCamera();
    static void cameraGo(float goBack, float leftRight);
    static void flush3DCamera();
    static vector<float> calLightScreen(float ratio);
    static void changeYj(float delta);
    static void changeDirection(float delta);
};


#endif
