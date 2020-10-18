//
// Created by Administrator on 2017/3/8 0008.
//

#include "CameraUtil.h"
#include <math.h>
#include "MatrixState3D.h"
#include "../square/My3DLayer.h"
#include "../square/ZFTManager.h"

float CameraUtil::degree=0;
float CameraUtil::yj=0;
float CameraUtil::camera9Para[9];

void CameraUtil::calCamera()
{
    if(ZFTManager::Box_pos!=TREE_NUMBER-1){
        camera9Para[3]=My3DLayer::mainBox->x;
        camera9Para[4]=My3DLayer::mainBox->y;
        camera9Para[5]=My3DLayer::mainBox->z;
        CameraUtil::camera9Para[0]=My3DLayer::mainBox->x-400;
        CameraUtil::camera9Para[1]=My3DLayer::mainBox->y+700;
        CameraUtil::camera9Para[2]=My3DLayer::mainBox->z-200;
    }
    else{
        camera9Para[3]=My3DLayer::mainBox->x;
        camera9Para[4]=100;
        camera9Para[5]=My3DLayer::mainBox->z;
        CameraUtil::camera9Para[0]=My3DLayer::mainBox->x-400;
        CameraUtil::camera9Para[1]=800;
        CameraUtil::camera9Para[2]=My3DLayer::mainBox->z-200;
    }
}
void CameraUtil::flushCameraToMatrix()
{
    //设置3D摄像机参数
    MatrixState3D::setCamera
            (
                    camera9Para[0],camera9Para[1],camera9Para[2],
                    camera9Para[3],camera9Para[4],camera9Para[5],
                    camera9Para[6],camera9Para[7],camera9Para[8]
            );
}