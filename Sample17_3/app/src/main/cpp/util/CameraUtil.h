//
// Created by Administrator on 2018/5/18.
//

#ifndef SAMPLE7_5_CAMERAUTIL_H
#define SAMPLE7_5_CAMERAUTIL_H


class CameraUtil {
public:
    static float num;
    static float cAngle;
    static float r;
    static float targetX;
    static float targetY;
    static float targetZ;
    static float upX;
    static float upY;
    static float upZ;
    static float ANGLE_MIN;
    static float ANGLE_MAX;
    static float CameraX;
    static float CameraY;
    static float CameraZ;
    static void calculateCamera(float angle);
};


#endif //SAMPLE7_5_CAMERAUTIL_H
