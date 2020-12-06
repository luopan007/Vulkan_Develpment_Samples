#include "CameraUtil.h"
#include <math.h>
#include "MatrixState3D.h"
#include <vector>
#include <stdlib.h>
#include "HelpFunction.h"
using namespace std;
float CameraUtil::degree = 0;
float CameraUtil::yj = 0;
float  CameraUtil::direction=0;
float CameraUtil::camera9Para[9];

float CameraUtil::tx = 0;
float CameraUtil::ty = 0;
float CameraUtil::tz = -20;
float CameraUtil::cx = 0;
float CameraUtil::cy = 0;
float CameraUtil::cz = 0;
float CameraUtil::upx = 0;
float CameraUtil::upy = 0;
float CameraUtil::upz = 0;
float LIGHT_POSITION[] = { 0.98f,11.27f,-27.6f,1 };
float UP_INIT[] = { 0,1,0,1 };
float TARGRT_INIT[] = { 0,0,-1,1 };
void CameraUtil::calCamera()
{
    float mat[16] ;
    Matrix::setIdentityM(mat, 0);
    Matrix::rotateM(mat, 0, direction, 0, 1, 0);
    Matrix::rotateM(mat, 0, yj, 1, 0, 0);
    float upResult[4] ;
    Matrix::multiplyMV(upResult, 0, mat, 0, UP_INIT, 0);
    upx = upResult[0];
    upy = upResult[1];
    upz = upResult[2];
    float targetResult[4];
    Matrix::multiplyMV(targetResult, 0, mat, 0, TARGRT_INIT, 0);
    tx = targetResult[0];
    ty = targetResult[1];
    tz = targetResult[2];
}
void CameraUtil::init3DCamera() {
    calCamera();
    flush3DCamera();
}
vector<float> CameraUtil::calLightScreen(float ratio) {//¼ÆËãÆÁÄ»¹âÔ´Î»ÖÃ
    float* mat = MatrixState3D::getFinalMatrix();
    float lightResult[4];
    Matrix::multiplyMV(lightResult, 0, mat, 0, LIGHT_POSITION, 0);
    if (abs(lightResult[2] / lightResult[3])>1.0)
    {
        return vector<float> {ratio * 2, 2};
    }
    else
    {
        return  vector<float> {ratio*lightResult[0] / lightResult[3], lightResult[1] / lightResult[3]};
    }
}
void CameraUtil::cameraGo(float goBack, float leftRight)
{
    float xStep = float(-goBack*sin(degree*3.1415926535898 / 180) - leftRight*sin((degree + 90)*3.1415926535898 / 180));
    float zStep = float(-goBack*cos(degree*3.1415926535898 / 180) - leftRight*cos((degree + 90)*3.1415926535898 / 180));
    tx = tx + xStep;
    tz = tz + zStep;
    calCamera();
}

void CameraUtil::flush3DCamera()
{
    MatrixState3D::setCamera(cx, cy, cz, tx, ty, tz, upx, upy, upz);
}

void CameraUtil::changeDirection(float delta) {
    direction = direction + delta;
    if (direction<0)
    {
        direction = direction + 360;
    }
    if (direction>360)
    {
        direction = direction - 360;
    }
    calCamera();
}
void CameraUtil::changeYj(float delta){
    yj = yj + delta;
    if (yj>88)
    {
        yj = 88;
    }
    if (yj<-88)
    {
        yj = -88;
    }
    calCamera();
}
