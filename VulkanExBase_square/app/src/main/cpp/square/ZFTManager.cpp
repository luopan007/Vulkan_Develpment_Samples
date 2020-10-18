#include "ZFTManager.h"
#include "My3DLayer.h"
#include "../util/MatrixState3D.h"
#include "../util/CameraUtil.h"
#include "../bndev/MyDraw.h"
#include "../bndev/MyVulkanManager.h"
#include <vector>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define translatecount 10//摄像机与方块x方向移动距离矫正
#define translatecount1 15//摄像机与方块x方向移动距离
#define angleflag 89 //旋转矫正值
#define attachscale_1 0.3
#define attachscale_2 0.18
#define attachrotate_1 40
#define attachrotate_2 65
#define Random() ((rand()%10000000)/10000000.0f)
int angle = 1;
float ZFTManager::MainScalez=1.0;
float ZFTManager::MainScaley=1.0;
float ZFTManager::MainScalex=1.0;
bool ZFTManager::life = true;
int ZFTManager::Diefoncution=0;
int ZFTManager::Box_pos = 0;
int Wincount=0;
float attachscale[8]={
        Random()*Random()*Random(),Random()*Random()*Random(),Random()*Random()*Random(),Random()*Random()*Random(),
        Random()*Random()*Random(),Random()*Random()*Random(),Random()*Random()*Random(),Random()*Random()*Random()
};
//float attachscale0 =Random()*Random()*Random();
void  ZFTManager::Update_mainbox() {
    if (My3DLayer::mainBox->Goorientation ==Goorientationright) {//右拐实现滚动效果
        My3DLayer::mainBox->Rangle = angle;
        My3DLayer::mainBox->x -= translatecount1;
        CameraUtil::camera9Para[0] -= translatecount1;
        CameraUtil::camera9Para[3] = My3DLayer::mainBox->x;
        angle -= translatecount1;
        if (angle == -angleflag) {
            My3DLayer::mainBox->Rz = 0;
            My3DLayer::mainBox->Rangle = 0;
            My3DLayer::mainBox->x -= translatecount;
            CameraUtil::camera9Para[0] -= translatecount;
            CameraUtil::camera9Para[3] = My3DLayer::mainBox->x;
            angle = 1;
            My3DLayer::mainBox->Rz = 0;
            My3DLayer::mainBox->Goorientation = Goorientationinit;
            My3DLayer::BoxPos_licence=true;
            My3DLayer::attachbox_draw_licence[ZFTManager::Box_pos+1]=true;
        }
    }

    if (My3DLayer::mainBox->Goorientation == Goorientationstright) {//直走实现滚动效果
        My3DLayer::mainBox->Rangle = angle;
        My3DLayer::mainBox->z += translatecount1;
        CameraUtil::camera9Para[2] += translatecount1;
        CameraUtil::camera9Para[5] = My3DLayer::mainBox->z;
        angle -= translatecount1;
        if (angle == -angleflag) {
            My3DLayer::mainBox->Rx = 0;
            My3DLayer::mainBox->Rangle = 0;
            My3DLayer::mainBox->z += translatecount;
            CameraUtil::camera9Para[2] += translatecount;
            CameraUtil::camera9Para[5] = My3DLayer::mainBox->z;
            angle = 1;
            My3DLayer::mainBox->Rx = 0;
            My3DLayer::mainBox->Goorientation = Goorientationinit;
            My3DLayer::BoxPos_licence=true;
            My3DLayer::attachbox_draw_licence[ZFTManager::Box_pos+1]=true;
        }
    }
    if (My3DLayer::mainBox->Goorientation == Goorientationleft) {//左实现滚动效果
        My3DLayer::mainBox->Rangle = -angle;
        My3DLayer::mainBox->x += translatecount1;
        CameraUtil::camera9Para[0] += translatecount1;
        CameraUtil::camera9Para[3] = My3DLayer::mainBox->x;
        angle -= translatecount1;
        if (angle == -angleflag) {
            My3DLayer::mainBox->Rz = 0;
            My3DLayer::mainBox->Rangle = 0;
            My3DLayer::mainBox->x += translatecount;
            CameraUtil::camera9Para[0] += translatecount;
            CameraUtil::camera9Para[3] = My3DLayer::mainBox->x;
            angle = 1;
            My3DLayer::mainBox->Rz = 0;
            My3DLayer::mainBox->Goorientation = Goorientationinit;
            My3DLayer::BoxPos_licence=true;
            My3DLayer::attachbox_draw_licence[ZFTManager::Box_pos+1]=true;
        }
    }
    if(My3DLayer::BoxPos_licence){//方块位置记录增加
        My3DLayer::BoxPos++;
//        if(My3DLayer::BoxPos>=1){
        My3DLayer::BoxPos_licence=false;
//            My3DLayer::BoxPos=0;
        ZFTManager::Box_pos++;
//        }
    }
    for(int i=1;i<ATTACH_BOX_NUMBER;i++){
        if(My3DLayer::attachbox_draw_licence[i]==true){
            if(My3DLayer::attachbox_draw_count[i]<=20){
                MatrixState3D::pushMatrix();
                MatrixState3D::translate(My3DLayer::attachBox[i]->x-attachrotate_1,My3DLayer::attachBox[i]->y-10,My3DLayer::attachBox[i]->z-attachrotate_1);
                MatrixState3D::scale(attachscale[0],attachscale[0],attachscale[0]);
                My3DLayer::attachBox[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                                  &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::attachBox[i]->texturename)]));
                MatrixState3D::popMatrix();
                //______________________________________________________________________________________
                MatrixState3D::pushMatrix();
                MatrixState3D::translate(My3DLayer::attachBox2[i]->x-attachrotate_2,My3DLayer::attachBox2[i]->y-10,My3DLayer::attachBox2[i]->z-attachrotate_2);
                MatrixState3D::scale(attachscale[1],attachscale[1],attachscale[1]);
                My3DLayer::attachBox2[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                                   &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::attachBox2[i]->texturename)]));
                MatrixState3D::popMatrix();
                //______________________________________________________________________________________
                MatrixState3D::pushMatrix();
                MatrixState3D::translate(My3DLayer::attachBox3[i]->x+attachrotate_1,My3DLayer::attachBox3[i]->y-10,My3DLayer::attachBox3[i]->z+attachrotate_1);
                MatrixState3D::scale(attachscale[2],attachscale[2],attachscale[2]);
                My3DLayer::attachBox3[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                                   &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::attachBox3[i]->texturename)]));
                MatrixState3D::popMatrix();
                //______________________________________________________________________________________
                MatrixState3D::pushMatrix();
                MatrixState3D::translate(My3DLayer::attachBox4[i]->x+attachrotate_2,My3DLayer::attachBox4[i]->y-10,My3DLayer::attachBox4[i]->z+attachrotate_2);
                MatrixState3D::scale(attachscale[3],attachscale[3],attachscale[3]);
                My3DLayer::attachBox4[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                                   &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::attachBox4[i]->texturename)]));
                MatrixState3D::popMatrix();
                //______________________________________________________________________________________
                MatrixState3D::pushMatrix();
                MatrixState3D::translate(My3DLayer::attachBox5[i]->x-attachrotate_1,My3DLayer::attachBox5[i]->y-10,My3DLayer::attachBox5[i]->z+attachrotate_1);
                MatrixState3D::scale(attachscale[4],attachscale[4],attachscale[4]);
                My3DLayer::attachBox5[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                                   &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::attachBox5[i]->texturename)]));
                MatrixState3D::popMatrix();
                //______________________________________________________________________________________
                MatrixState3D::pushMatrix();
                MatrixState3D::translate(My3DLayer::attachBox6[i]->x-attachrotate_2,My3DLayer::attachBox6[i]->y-10,My3DLayer::attachBox6[i]->z+attachrotate_2);
                MatrixState3D::scale(attachscale[5],attachscale[5],attachscale[5]);
                My3DLayer::attachBox6[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                                   &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::attachBox6[i]->texturename)]));
                MatrixState3D::popMatrix();
                //______________________________________________________________________________________

                MatrixState3D::pushMatrix();
                MatrixState3D::translate(My3DLayer::attachBox7[i]->x+attachrotate_1,My3DLayer::attachBox7[i]->y-10,My3DLayer::attachBox7[i]->z-attachrotate_1);
                MatrixState3D::scale(attachscale[6],attachscale[6],attachscale[6]);
                My3DLayer::attachBox7[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                                   &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::attachBox7[i]->texturename)]));
                MatrixState3D::popMatrix();
                //______________________________________________________________________________________
                MatrixState3D::pushMatrix();
                MatrixState3D::translate(My3DLayer::attachBox8[i]->x+attachrotate_2,My3DLayer::attachBox8[i]->y-10,My3DLayer::attachBox8[i]->z-attachrotate_2);
                MatrixState3D::scale(attachscale[7],attachscale[7],attachscale[7]);
                My3DLayer::attachBox8[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                                   &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::attachBox8[i]->texturename)]));
                MatrixState3D::popMatrix();
            }
            else{
                My3DLayer::attachbox_draw_licence[i]=false;
                for(int i=0;i<8;i++){
                    attachscale[i]=Random()*Random()*Random();
                }
                for(int i=0;i<8;i++){
                    if(attachscale[i]>=0.5f)
                        attachscale[i]-=0.3f;
                    else if(attachscale[i]<0.5f)
                        attachscale[i]+=0.2f;
                }
            }
            My3DLayer::attachbox_draw_count[i]++;
        }
    }
}
void ZFTManager::ZFTDiCiDie()//地刺刺穿方块死亡方式
{
    if(My3DLayer::mainBox->y>=-50)
        My3DLayer::mainBox->y-=5;
}
void ZFTManager::ZFTJiaZiDie()//夹子夹方块死亡方式
{
    if(My3DLayer::flag[Box_pos]==0||My3DLayer::flag[Box_pos]==2){//方块横向死亡
        if(ZFTManager::MainScalez>=0.25){
            ZFTManager::MainScalez-=0.15;
        }
        else
            My3DLayer::mainBox->y=0;
    }
    else if(My3DLayer::flag[Box_pos]==1){//方块纵向死亡
        if(ZFTManager::MainScalex>=0.25){
            ZFTManager::MainScalex-=0.15;
        }
        else
            My3DLayer::mainBox->y=-50;
    }
}
void ZFTManager::ZFTDiaoLuoDie()//掉落压扁方块死亡方式
{
    if(My3DLayer::mainBox->y>=-50)
        My3DLayer::mainBox->y-=5;
}
void ZFTManager::ZFTYiDongDie()//掉落压扁方块死亡方式
{
    if(My3DLayer::mainBox->y>=-50)
        My3DLayer::mainBox->y-=5;
}
void ZFTManager::Win(){
    if(Box_pos==TREE_NUMBER-1){
        if(Wincount<90){
            My3DLayer::mainBox->y+=1;
            My3DLayer::mainBox->Ry=1;
            My3DLayer::mainBox->Rangle=Wincount;
        }else if(Wincount<180){
            My3DLayer::mainBox->y-=1;
            My3DLayer::mainBox->Ry=1;
            My3DLayer::mainBox->Rangle=Wincount;
        }
        else
            Wincount=0;
        Wincount++;
    }
//    if(Box_pos==1){
//        My3DLayer::BoxPos=0;                                        //方块位置记录计时
//        My3DLayer::BoxPos_licence=false;                           //方块位置记录开关
//        ZFTManager::life=true;
//        ZFTManager::Box_pos=0;
//        My3DLayer::mainBox->setPosition3D(0,100,0);
//        My3DLayer::Level=2;
//        My3DLayer::initLevel();
//        MyDraw::InitDrawobject();
//    }
}