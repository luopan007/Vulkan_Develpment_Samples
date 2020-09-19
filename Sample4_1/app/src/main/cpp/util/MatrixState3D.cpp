#include "MatrixState3D.h"
#include <math.h>

float MatrixState3D::vulkanClipMatrix[16];
float MatrixState3D::currMatrix[16];
float MatrixState3D::mProjMatrix[16];
float MatrixState3D::mVMatrix[16];
float MatrixState3D::mMVPMatrix[16];
float MatrixState3D::mStack[10][16];
int MatrixState3D::stackTop = -1;
float MatrixState3D::cx, MatrixState3D::cy, MatrixState3D::cz;

void MatrixState3D::setInitStack() {                    //初始化基本变换矩阵的方法
    Matrix::setIdentityM(currMatrix, 0);
    vulkanClipMatrix[0] = 1.0f;
    vulkanClipMatrix[1] = 0.0f;
    vulkanClipMatrix[2] = 0.0f;
    vulkanClipMatrix[3] = 0.0f;
    vulkanClipMatrix[4] = 0.0f;
    vulkanClipMatrix[5] = -1.0f;
    vulkanClipMatrix[6] = 0.0f;
    vulkanClipMatrix[7] = 0.0f;
    vulkanClipMatrix[8] = 0.0f;
    vulkanClipMatrix[9] = 0.0f;
    vulkanClipMatrix[10] = 0.5f;
    vulkanClipMatrix[11] = 0.0f;
    vulkanClipMatrix[12] = 0.0f;
    vulkanClipMatrix[13] = 0.0f;
    vulkanClipMatrix[14] = 0.5f;
    vulkanClipMatrix[15] = 1.0f;
}

void MatrixState3D::pushMatrix() {                    //保护基本变换矩阵（保护现场）的方法
    stackTop++;                                //栈顶元素索引自增
    for (int i = 0; i < 16; i++) {
        mStack[stackTop][i] = currMatrix[i];            //保存当前变换矩阵的元素值
    }
}

void MatrixState3D::popMatrix() {                    //恢复基本变换矩阵（恢复现场）的方法
    for (int i = 0; i < 16; i++) {
        currMatrix[i] = mStack[stackTop][i];            //将栈顶数组取值填入当前基本变换矩阵
    }
    stackTop--;
}                                //栈顶元素索引自减
void MatrixState3D::translate(float x, float y, float z) {        //执行平移变换的方法
    Matrix::translateM(currMatrix, 0, x, y, z);        //调用Matrix类的translateM方法完成平移变换
}

void MatrixState3D::rotate(float angle, float x, float y, float z) {    //执行旋转变换的方法
    Matrix::rotateM(currMatrix, 0, angle, x, y, z);        //调用Matrix类的rotateM方法完成旋转变换
}

void MatrixState3D::scale(float x, float y, float z) {                //执行缩放变换的方法
    Matrix::scaleM(currMatrix, 0, x, y, z);            //调用Matrix类的scaleM方法完成缩放变换
}

void MatrixState3D::setCamera(                            //设置摄像机的方法
        float cx, float cy, float cz, float tx, float ty, float tz,                    //摄像机位置及目标点坐标
        float upx, float upy, float upz) {                                //up向量坐标
    MatrixState3D::cx = cx;
    MatrixState3D::cy = cy;
    MatrixState3D::cz = cz;//记录摄像机位置坐标
    Matrix::setLookAtM(                                    //调用Matrix类的setLookAtM
            mVMatrix, 0, cx, cy, cz, tx, ty, tz, upx, upy, upz);                //方法计算摄像机矩阵
}

void MatrixState3D::setProjectFrustum(                         //设置透视投影参数的方法
        float left, float right, float bottom, float top, float near,
        float far) {         //透视投影六个参数
    Matrix::frustumM(mProjMatrix, 0, left, right, bottom, top, near, far);//调用frustumM方法完成操作
}

void MatrixState3D::setProjectOrtho(                            //设置正交参数的方法
        float left, float right, float bottom, float top, float near, float far) {        //正交投影六个参数
    Matrix::orthoM(mProjMatrix, 0, left, right, bottom, top, near, far); //调用orthoM方法完成操作
}

float *MatrixState3D::getFinalMatrix() {                         //获取总变换矩阵的方法
    Matrix::multiplyMM(mMVPMatrix, 0, mVMatrix, 0, currMatrix, 0); //当前基本变换矩阵与摄像机矩阵相乘
    Matrix::multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);      //进一步乘以投影矩阵
    Matrix::multiplyMM(mMVPMatrix, 0, vulkanClipMatrix, 0, mMVPMatrix, 0);//乘以标准设备空间调整矩阵
    return mMVPMatrix;                                          //返回总变换矩阵
}

float *MatrixState3D::getMMatrix() { return currMatrix; }                  //获取当前基本变换矩阵




