#include "My3DLayer.h"
#include "../square/ZFTManager.h"
#include "../bndev/MyDraw.h"
#include "../square/DiaoLuoManager.h"
#include "../square/DiCiBox.h"
#include "../square/JiaZiBox.h"
#include "../square/TanShe.h"
#include "../square/YiDongBox.h"
#include "../bndev/Touch2D.h"
int  My3DLayer::flag[TREE_NUMBER] = {1,1,1,1,1,1,1,1,1,1,            //关卡地板布局方式
                                     2,2,2,2,2,2,2,2,2,2,
                                     1,1,1,1,1,1,1,1,1,1,
                                     1,2,1,2,1,2,1,2,1,0,
                                     0,0,0,0,0,0,0,0,0,0};
int  My3DLayer::flag1[TREE_NUMBER] = {1,1,1,1,1,1,1,1,1,1,            //关卡1地板布局方式
                                     2,2,2,2,2,2,2,2,2,2,
                                     1,1,1,1,1,1,1,1,1,1,
                                     1,2,1,2,1,2,1,2,1,0,
                                     0,0,0,0,0,0,0,0,0,0};

//int  My3DLayer::flag1[TREE_NUMBER] = {0,0,0,0,0,0,0,0,0,0,            //关卡1地板布局方式
//                                        0,0,0,0,0,0,0,0,0,0,
//                                        0,0,0,0,0,0,0,0,0,0,
//                                        0,0,0,0,0,0,0,0,0,0,
//                                        0,0,0,0,0,0,0,0,0,0};



int  My3DLayer::flag2[TREE_NUMBER] = {1,1,1,1,1,0,0,0,0,0,          //关卡2地板布局方式
                                      1,1,1,1,1,2,2,2,2,2,
                                      2,2,2,2,2,1,1,1,1,1,
                                      0,0,0,0,0,0,0,0,0,0,
                                      1,1,1,1,1,1,1,1,1,1};
int  My3DLayer::flagGo[TREE_NUMBER] = {1,1,1,1,1,1,1,1,1,2,          //关卡1翻转路径
                                       2,2,2,2,2,2,2,2,2,1,
                                       1,1,1,1,1,1,1,1,1,1,
                                       2,1,2,1,2,1,2,1,0,0,
                                       0,0,0,0,0,0,0,0,0,0};
int  My3DLayer::flag1Go[TREE_NUMBER] = {1,1,1,1,1,1,1,1,1,2,          //关卡1翻转路径
                                       2,2,2,2,2,2,2,2,2,1,
                                       1,1,1,1,1,1,1,1,1,1,
                                       2,1,2,1,2,1,2,1,0,0,
                                       0,0,0,0,0,0,0,0,0,0};
int  My3DLayer::flag2Go[TREE_NUMBER] = {1,1,1,1,0,0,0,0,0,1,        //关卡2翻转路径
                                        1,1,1,1,2,2,2,2,2,2,
                                        2,2,2,2,1,1,1,1,1,0,
                                        0,0,0,0,0,0,0,0,0,1,
                                        1,1,1,1,1,1,1,1,1,1};
int My3DLayer::dicipos[DICI_NUMBER] = {4, 13,19,20,21,30,42};	    //关卡1地刺位置数组
int My3DLayer::jiazipos[JIAZI_NUMBER] = {7,8,11,25,48};			    //关卡1夹子位置数组
int My3DLayer::diaoluopos[DIAOLUO_NUMBER] = {15,27,36,40};			//关卡1掉落块位置数组
int My3DLayer::yidongpos[YIDONG_NUMBER] = {3};                      //关卡1移动位置
int My3DLayer::tanshepos[TANSHE_NUMBER]={5};

int My3DLayer::dicipos1[DICI_NUMBER] = {4, 13,19,20,21,30,42};	    //关卡1地刺位置数组
int My3DLayer::jiazipos1[JIAZI_NUMBER] = {7,8,11,25,48};			    //关卡1夹子位置数组
int My3DLayer::diaoluopos1[DIAOLUO_NUMBER] = {15,27,36,40};			//关卡1掉落块位置数组
int My3DLayer::yidongpos1[YIDONG_NUMBER] = {3};                      //关卡1移动位置
int My3DLayer::tanshepos1[TANSHE_NUMBER]={5};

int My3DLayer::dicipos2[DICI_NUMBER] = {4, 13,19,20,21,30,39};	    //关卡2地刺位置数组
int My3DLayer::jiazipos2[JIAZI_NUMBER] = {7,8,11,25,48};			    //关卡2夹子位置数组
int My3DLayer::diaoluopos2[DIAOLUO_NUMBER] = {15,27,36,41};			//关卡2掉落块位置数组
int My3DLayer::yidongpos2[YIDONG_NUMBER] = {3};                     //关卡1移动位置
int My3DLayer::tanshepos2[TANSHE_NUMBER]={5};
int My3DLayer::attachbox_draw_count[ATTACH_BOX_NUMBER]={0,0,0,0,0,0,0,0,0,0,    //附属方块绘制计时
                                                        0,0,0,0,0,0,0,0,0,0,
                                                        0,0,0,0,0,0,0,0,0,0,
                                                        0,0,0,0,0,0,0,0,0,0,
                                                        0,0,0,0,0,0,0,0,0,0
};

int  My3DLayer::BoxPos=0;                                        //方块位置记录计时
bool My3DLayer::BoxPos_licence=false;                           //方块位置记录开关
bool My3DLayer::attachbox_draw_licence[ATTACH_BOX_NUMBER]={0,0,0,0,0,0,0,0,0,0, //附属小方块绘制开关
                                                           0,0,0,0,0,0,0,0,0,0,
                                                           0,0,0,0,0,0,0,0,0,0,
                                                           0,0,0,0,0,0,0,0,0,0,
                                                           0,0,0,0,0,0,0,0,0,0
};
int My3DLayer::Level=1;                                         //初始关卡
void My3DLayer::initLevel(){                                    //初始关卡物体
    if(My3DLayer::Level>2){
        My3DLayer::Level=1;
    }
    if(My3DLayer::Level==1){
        for(int i=0;i<TREE_NUMBER;i++){                         //初始化关卡1道路
            My3DLayer::flag[i]=My3DLayer::flag1[i];
            My3DLayer::flagGo[i]=My3DLayer::flag1Go[i];
        }
        for(int i=0;i<DICI_NUMBER;i++){                         //初始化关卡1地刺
            My3DLayer::dicipos[i]=My3DLayer::dicipos1[i];
        }
        for(int i=0;i<JIAZI_NUMBER;i++){                        //初始化关卡1夹子
            My3DLayer::jiazipos[i]=My3DLayer::jiazipos1[i];
        }
        for(int i=0;i<DIAOLUO_NUMBER;i++){                      //初始化关卡1掉落
            My3DLayer::diaoluopos[i]=My3DLayer::diaoluopos1[i];
        }
        for(int i=0;i<YIDONG_NUMBER;i++){                       //初始化关卡1移动
            My3DLayer::yidongpos[i]=My3DLayer::yidongpos1[i];
        }
        for(int i=0;i<TANSHE_NUMBER;i++){                       //初始化关卡1弹射
            My3DLayer::tanshepos[i]=My3DLayer::tanshepos1[i];
        }
    }
    else if(My3DLayer::Level==2){
        for(int i=0;i<TREE_NUMBER;i++){
            My3DLayer::flag[i]=My3DLayer::flag2[i];              //改变道路
            My3DLayer::flagGo[i]=My3DLayer::flag2Go[i];
        }
        for(int i=0;i<DICI_NUMBER;i++){
            My3DLayer::dicipos[i]=My3DLayer::dicipos2[i];
        }
        for(int i=0;i<JIAZI_NUMBER;i++){
            My3DLayer::jiazipos[i]=My3DLayer::jiazipos2[i];
        }
        for(int i=0;i<DIAOLUO_NUMBER;i++){
            My3DLayer::diaoluopos[i]=My3DLayer::diaoluopos2[i];
        }
        for(int i=0;i<YIDONG_NUMBER;i++){
            My3DLayer::yidongpos[i]=My3DLayer::yidongpos2[i];
        }
        for(int i=0;i<TANSHE_NUMBER;i++){
            My3DLayer::tanshepos[i]=My3DLayer::tanshepos2[i];
        }
    }

}
void  My3DLayer::NextLevel(){
    for(int i=1;i<ATTACH_BOX_NUMBER;i++){               //复位附属方块
        My3DLayer::attachbox_draw_licence[i]=0;
        My3DLayer::attachbox_draw_count[i]=0;
    }
    My3DLayer::BoxPos_licence=false;                           //方块位置记录开关
    My3DLayer::mainBox->setPosition3D(0,100,0);                 //初始化主方块位置
    My3DLayer::mainBox->Ry=0;                                   //初始化主方块旋转轴与旋转角
    My3DLayer::mainBox->Rx=0;
    My3DLayer::mainBox->Rz=0;
    My3DLayer::mainBox->Rangle=0;
    My3DLayer::Level+=1;                                         //关卡标志
    DiaoLuoManager::DiaoLuoIndex=0;                             //掉落格子数归零
    DiCiBox::DiCiIndex=0;                                       //地刺格子数归零
    JiaZiBox::JiaZiIndex=0;                                     //夹子格子数归零
    YiDongBox::YiDongIndex=0;                                   //移动格子数归零
    My3DLayer::initLevel();                                     //更新为当前关卡物体脚本数组
    MyDraw::InitDrawobject();                                   //改变当前关卡物体的位置
    My3DLayer::BoxPos=0;                                        //方块位置记录计时
    ZFTManager::life=true;                                      //主方块的生存状态
    ZFTManager::Box_pos=0;                                      //小方块位置记录归零
    Touch2D::Button_start=false;
}
void  My3DLayer::RepeatLevel() {
    for(int i=1;i<ATTACH_BOX_NUMBER;i++){                       //复位附属方块
        My3DLayer::attachbox_draw_licence[i]=0;
        My3DLayer::attachbox_draw_count[i]=0;
    }
    My3DLayer::BoxPos_licence=false;                           //方块位置记录开关
    My3DLayer::mainBox->setPosition3D(0,100,0);                  //复位方块位置
    ZFTManager::MainScalex=1;                                   //复位方块缩放大大小
    ZFTManager::MainScaley=1;
    ZFTManager::MainScalez=1;
    DiCiBox::DiCiIndex=0;                                       //地刺格子个数复位
    JiaZiBox::JiaZiIndex=0;                                     //夹子格子个数复位
    YiDongBox::YiDongIndex=0;                                   //移动格子个数复位
    My3DLayer::BoxPos=0;                                        //方块位置记录计时
    ZFTManager::life=true;                                      //方块生命恢复
    ZFTManager::Box_pos=0;                                       //小方块位置记录归零
    Touch2D::Button_resume=false;
}