#include "MyDraw.h"
#include "../square/My3DLayer.h"
#include "../square/Square3D.h"
#include "../square/DiCiBox.h"
#include "../square/JiaZiBox.h"
#include "../square/YiDongBox.h"
#include "../square/DiaoLuoManager.h"
#include "../square/TanShe.h"
#include "MatrixState3D.h"
#include "MatrixState2D.h"
#include "../square/ZFTManager.h"
#include "../bndev/Touch2D.h"
#include "../util/CameraUtil.h"
//________________MyVulkanManager类中成员变量更改
void MyDraw::CreateDrawobject(){
    //地面
    My3DLayer::planeForDraw=Square3D::create("model/plane.obj","texture/floor.pkm");
    //加入路径模型-------------------------------------------------
    My3DLayer::sp3Tree[0]=Square3D::create("model/cft.obj", "texture/cft.pkm");
    for(int i=1;i < TREE_NUMBER;i++) {
        My3DLayer::sp3Tree[i] = Square3D::create("model/cft.obj", "texture/cft.pkm");
    }
    for(int i = 0; i < DICI_NUMBER; i++)
    {
        //地刺
        My3DLayer::sp3Dici[i]=Square3D::create("model/dici.obj","texture/dici.pkm");
    }
    for(int i = 0; i < JIAZI_NUMBER; i++)
    {
        //夹子
        My3DLayer::sp3JiaziLeft[i]=Square3D::create("model/jiazike.obj","texture/jiazi.pkm");
        My3DLayer::sp3JiaziRight[i]=Square3D::create("model/jiazike.obj","texture/jiazi.pkm");
        My3DLayer::sp3JiaziLeftClose[i]=Square3D::create("model/jiazijia.obj","texture/dici.pkm");
        My3DLayer::sp3JiaziRightClose[i]=Square3D::create("model/jiazijia.obj","texture/dici.pkm");
    }
    for(int i=0;i<DIAOLUO_NUMBER;i++){//掉落
        My3DLayer::sp3DiaoLuo[i]=Square3D::create("model/diaoluo.obj","texture/cft.pkm");
        My3DLayer::sp3DiaoLuoshadow[i]=Square3D::create("model/zft.obj","texture/cftshadow.pkm");
    }
    for(int i=0;i<YIDONG_NUMBER;i++){//移动
        My3DLayer::sp3YiDong[i]=Square3D::create("model/cft.obj","texture/cft.pkm");
    }
    //创建主方块
    My3DLayer::mainBox=Square3D::create("model/zft.obj","texture/zft.pkm");
    //附属方块
    for(int i=0;i<ATTACH_BOX_NUMBER;i++){
        My3DLayer::attachBox[i]=Square3D::create("model/attach.obj","texture/zft.pkm");
        My3DLayer::attachBox2[i]=Square3D::create("model/attach.obj","texture/zft.pkm");
        My3DLayer::attachBox3[i]=Square3D::create("model/attach.obj","texture/zft.pkm");
        My3DLayer::attachBox4[i]=Square3D::create("model/attach.obj","texture/zft.pkm");
        My3DLayer::attachBox5[i]=Square3D::create("model/attach.obj","texture/zft.pkm");
        My3DLayer::attachBox6[i]=Square3D::create("model/attach.obj","texture/zft.pkm");
        My3DLayer::attachBox7[i]=Square3D::create("model/attach.obj","texture/zft.pkm");
        My3DLayer::attachBox8[i]=Square3D::create("model/attach.obj","texture/zft.pkm");
    }
    //旗子
    My3DLayer::qizi=Square3D::create("model/qizi.obj","texture/qizi.pkm");
    for(int i=0;i<QIZIGUN_NUMBER;i++){
        My3DLayer::qizigun[i]=Square3D::create("model/qizigun.obj","texture/qizigun.pkm");
    }
    //弹射
    for(int i=0;i<TANSHE_NUMBER;i++){
        My3DLayer::tanshe[i]=Square3D::create("model/tanshe.obj","texture/tanshe.pkm");
    }
}

void MyDraw::InitDrawobject() {
    //道路绘制
    My3DLayer::sp3Tree[0]->setPosition3D(0,40,0);
    for(int i=1;i < TREE_NUMBER;i++) {
        switch ( My3DLayer::flag[i]) {
            case 0: // x轴方向 向右
                My3DLayer::sp3Tree[i]->setPosition3D(My3DLayer::sp3Tree[i - 1]->x - 100, 40,
                                                     My3DLayer::sp3Tree[i - 1]->z);
                break;
            case 1: //z轴方向 向前
                My3DLayer::sp3Tree[i]->setPosition3D(My3DLayer::sp3Tree[i - 1]->x, 40,
                                                     My3DLayer::sp3Tree[i - 1]->z + 100);
                break;
            case 2: // x轴方向 向左
                My3DLayer::sp3Tree[i]->setPosition3D(My3DLayer::sp3Tree[i - 1]->x + 100, 40,
                                                     My3DLayer::sp3Tree[i - 1]->z);
                break;
        }
    }
    //地刺绘制
    for(int i = 0; i < DICI_NUMBER; i++)
    {
        LOGE("地刺 ： %d", My3DLayer::dicipos[i]);

        My3DLayer::sp3Dici[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::dicipos[i]]->x, 38,
                                             My3DLayer::sp3Tree[My3DLayer::dicipos[i]]->z);
    }
    //夹子绘制
    for(int i = 0; i < JIAZI_NUMBER; i++)
    {
        My3DLayer::sp3JiaziLeft[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->x, 30,
                                                  My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->z);
        My3DLayer::sp3JiaziRight[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->x, 30,
                                                   My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->z);

        My3DLayer::sp3JiaziLeftClose[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->x, 90,
                                                       My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->z);
        My3DLayer::sp3JiaziRightClose[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->x, 90,
                                                        My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->z);
    }
    for(int i=0;i<DIAOLUO_NUMBER;i++){//地落
        My3DLayer::sp3DiaoLuo[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::diaoluopos[i]]->x, 80,
                                                My3DLayer::sp3Tree[My3DLayer::diaoluopos[i]]->z);
        My3DLayer::sp3DiaoLuoshadow[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::diaoluopos[i]]->x, 65,
                                                      My3DLayer::sp3Tree[My3DLayer::diaoluopos[i]]->z);
}
    for(int i=0;i<YIDONG_NUMBER;i++){//移动
        My3DLayer::sp3YiDong[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::yidongpos[i]]->x, 60,
                                               My3DLayer::sp3Tree[My3DLayer::yidongpos[i]]->z);
    }
    My3DLayer::mainBox->setPosition3D(0,100,0);
    //附属方块
    for(int i=0;i<ATTACH_BOX_NUMBER;i++){
        My3DLayer::attachBox[i]->setPosition3D(My3DLayer::sp3Tree[i]->x,70,My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox2[i]->setPosition3D(My3DLayer::sp3Tree[i]->x,70,My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox3[i]->setPosition3D(My3DLayer::sp3Tree[i]->x,70,My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox4[i]->setPosition3D(My3DLayer::sp3Tree[i]->x,70,My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox5[i]->setPosition3D(My3DLayer::sp3Tree[i]->x,70,My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox6[i]->setPosition3D(My3DLayer::sp3Tree[i]->x,70,My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox7[i]->setPosition3D(My3DLayer::sp3Tree[i]->x,70,My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox8[i]->setPosition3D(My3DLayer::sp3Tree[i]->x,70,My3DLayer::sp3Tree[i]->z);
    }
    //旗子
    My3DLayer::qizi->setPosition3D(0,120,0);
    for(int i=0;i<QIZIGUN_NUMBER;i++){
        My3DLayer::qizigun[i]->setPosition3D(0,120,0);
    }
    //弹射
    for(int i=0;i<TANSHE_NUMBER;i++){
        My3DLayer::tanshe[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::tanshepos[i]]->x, 65,
                                            My3DLayer::sp3Tree[My3DLayer::tanshepos[i]]->z);
    }
}

void MyDraw::Draw3Dobject() {

    DiCiBox::DiCiTimeUpdate(); //更新地刺
    JiaZiBox::JiaZiTimeUpdate(); //更新夹子
    DiaoLuoManager::DiaoLuoTimeUpdate(); //更新掉落
    YiDongBox::YiDongTimeUpdate(); //更新移动
    //绘制地面
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0,0,1500);
    MatrixState3D::scale(6,6,6);
    // 游戏中 蓝白色 地板
    My3DLayer::planeForDraw->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                      &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::planeForDraw->texturename)]));
    MatrixState3D::popMatrix();

    //绘制道路
    for(int i=0;i < TREE_NUMBER;i++) {
        for(int j=0;j<YIDONG_NUMBER;j++){
            if(i!=My3DLayer::yidongpos[j]){
                MatrixState3D::pushMatrix();
                MatrixState3D::translate(My3DLayer::sp3Tree[i]->x,My3DLayer::sp3Tree[i]->y,My3DLayer::sp3Tree[i]->z);
                My3DLayer::sp3Tree[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                                &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::sp3Tree[i]->texturename)]));
                MatrixState3D::popMatrix();
            }
        }
    }
    //绘制地刺
    for(int i=0;i < DICI_NUMBER;i++) {
        MatrixState3D::pushMatrix();
        MatrixState3D::translate(My3DLayer::sp3Dici[i]->x,My3DLayer::sp3Dici[i]->y + DiCiBox::DiCiUpDown(),My3DLayer::sp3Dici[i]->z);
        My3DLayer::sp3Dici[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                        &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::sp3Dici[i]->texturename)]));
        MatrixState3D::popMatrix();
    }
    //绘制夹子
    for(int i=0;i < JIAZI_NUMBER;i++) {
        //左右夹子
        MatrixState3D::pushMatrix();
        if(My3DLayer::flag[My3DLayer::jiazipos[i]] != 1){
            MatrixState3D::translate(My3DLayer::sp3JiaziLeft[i]->x,My3DLayer::sp3JiaziLeft[i]->y,My3DLayer::sp3JiaziLeft[i]->z -JiaZiBox::JiaZiUpDown());
            MatrixState3D::rotate(90.0,0.0,1.0,0.0);
        }else{
            MatrixState3D::translate(My3DLayer::sp3JiaziLeft[i]->x -JiaZiBox::JiaZiUpDown(),My3DLayer::sp3JiaziLeft[i]->y,My3DLayer::sp3JiaziLeft[i]->z);
        }
        My3DLayer::sp3JiaziLeft[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                             &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::sp3JiaziLeft[i]->texturename)]));
        MatrixState3D::popMatrix();

        MatrixState3D::pushMatrix();
        if(My3DLayer::flag[My3DLayer::jiazipos[i]] != 1){
            MatrixState3D::translate(My3DLayer::sp3JiaziRight[i]->x,My3DLayer::sp3JiaziRight[i]->y,My3DLayer::sp3JiaziRight[i]->z +JiaZiBox::JiaZiUpDown());
            MatrixState3D::rotate(90.0,0.0,1.0,0.0);
        }else{
            MatrixState3D::translate(My3DLayer::sp3JiaziRight[i]->x +JiaZiBox::JiaZiUpDown(),My3DLayer::sp3JiaziRight[i]->y,My3DLayer::sp3JiaziRight[i]->z);
        }
        My3DLayer::sp3JiaziRight[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                             &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::sp3JiaziRight[i]->texturename)]));
        MatrixState3D::popMatrix();
        //左右夹子

        //左右夹子的齿轮
        MatrixState3D::pushMatrix();
        if(My3DLayer::flag[My3DLayer::jiazipos[i]] != 1){
            MatrixState3D::translate(My3DLayer::sp3JiaziLeftClose[i]->x,My3DLayer::sp3JiaziLeftClose[i]->y,My3DLayer::sp3JiaziLeftClose[i]->z -JiaZiBox::JiaZiUpDown());
            MatrixState3D::rotate(90,1,0,0);
            MatrixState3D::rotate(90-JiaZiBox::JiaZiUpDown()*5,0,1,0);
        }else{
            MatrixState3D::translate(My3DLayer::sp3JiaziLeftClose[i]->x -JiaZiBox::JiaZiUpDown(),My3DLayer::sp3JiaziLeftClose[i]->y,My3DLayer::sp3JiaziLeftClose[i]->z);
            MatrixState3D::rotate(90,0,0,1);
            MatrixState3D::rotate(90-JiaZiBox::JiaZiUpDown()*5,0,1,0);
        }

        MatrixState3D::scale(0.4f,0.4f,0.4f);
        My3DLayer::sp3JiaziLeftClose[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                             &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::sp3JiaziLeftClose[i]->texturename)]));
        MatrixState3D::popMatrix();
        //左右夹子的齿轮
    }

    //绘制掉落
    for(int i=0;i < DIAOLUO_NUMBER;i++) {
            MatrixState3D::pushMatrix();
            MatrixState3D::translate(My3DLayer::sp3DiaoLuoshadow[i]->x,My3DLayer::sp3DiaoLuoshadow[i]->y ,My3DLayer::sp3DiaoLuoshadow[i]->z);
            MatrixState3D::scale(1.2f-DiaoLuoManager::DiaoLuoUpDown()*0.01f,0.02f,1.2f-DiaoLuoManager::DiaoLuoUpDown()*0.01f);
            My3DLayer::sp3DiaoLuoshadow[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                                 &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::sp3DiaoLuoshadow[i]->texturename)]));
            MatrixState3D::popMatrix();

            MatrixState3D::pushMatrix();
            MatrixState3D::translate(My3DLayer::sp3DiaoLuo[i]->x,My3DLayer::sp3DiaoLuo[i]->y + DiaoLuoManager::DiaoLuoUpDown(),My3DLayer::sp3DiaoLuo[i]->z);
            My3DLayer::sp3DiaoLuo[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                               &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::sp3DiaoLuo[i]->texturename)]));
            MatrixState3D::popMatrix();


    }
    //  绘制移动平板
    for(int i=0;i < YIDONG_NUMBER;i++) {
        MatrixState3D::pushMatrix();
        if(My3DLayer::flag[My3DLayer::yidongpos[i]] != 1){
            MatrixState3D::translate(My3DLayer::sp3YiDong[i]->x,My3DLayer::sp3YiDong[i]->y ,My3DLayer::sp3YiDong[i]->z+ YiDongBox::YiDongUpDown());
            MatrixState3D::rotate(90.0,0.0,1.0,0.0);
        }else{
            MatrixState3D::translate(My3DLayer::sp3YiDong[i]->x+ YiDongBox::YiDongUpDown(),My3DLayer::sp3YiDong[i]->y ,My3DLayer::sp3YiDong[i]->z);
        }
        MatrixState3D::scale(1.0f,0.05f,1.0f);
        My3DLayer::sp3YiDong[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                           &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::sp3YiDong[i]->texturename)]));
        MatrixState3D::popMatrix();

    }
    //起点旗子
    MatrixState3D::pushMatrix();
    if(My3DLayer::flag[2] != 1){
        MatrixState3D::translate(My3DLayer::sp3Tree[2]->x,My3DLayer::qizi->y,My3DLayer::sp3Tree[2]->z );
    }else{
        MatrixState3D::translate(My3DLayer::sp3Tree[2]->x ,My3DLayer::qizi->y,My3DLayer::sp3Tree[2]->z);
        MatrixState3D::rotate(90.0,0.0,1.0,0.0);
    }
        MatrixState3D::scale(1.1,1.1,1.1);
    My3DLayer::qizi->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                         &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::qizi->texturename)]));
    MatrixState3D::popMatrix();
    //起点旗子棍
    for(int i=0;i<QIZIGUN_NUMBER;i++){
        MatrixState3D::pushMatrix();
        if(My3DLayer::flag[2] != 1){
            if(i==0)
                MatrixState3D::translate(My3DLayer::sp3Tree[2]->x,My3DLayer::qizi->y,My3DLayer::sp3Tree[2]->z );
            else{
                MatrixState3D::translate(My3DLayer::sp3Tree[2]->x,My3DLayer::qizi->y,My3DLayer::sp3Tree[2]->z );
                MatrixState3D::rotate(180.0,0.0,1.0,0.0);
            }
        }else{
            if(i==0){
                MatrixState3D::translate(My3DLayer::sp3Tree[2]->x ,My3DLayer::qizi->y,My3DLayer::sp3Tree[2]->z);
                MatrixState3D::rotate(90.0,0.0,1.0,0.0);
            }
            else{
                MatrixState3D::translate(My3DLayer::sp3Tree[2]->x,My3DLayer::qizi->y,My3DLayer::sp3Tree[2]->z );
                MatrixState3D::rotate(-90.0,0.0,1.0,0.0);
            }
        }
        MatrixState3D::scale(1.1,1.1,1.1);
        My3DLayer::qizigun[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                  &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::qizigun[i]->texturename)]));
        MatrixState3D::popMatrix();
    }
    //弹射
    for(int i=0;i<TANSHE_NUMBER;i++){
        MatrixState3D::pushMatrix();
        MatrixState3D::translate(My3DLayer::tanshe[i]->x,My3DLayer::tanshe[i]->y,My3DLayer::tanshe[i]->z);
        MatrixState3D::rotate(My3DLayer::tanshe[i]->Rangle,My3DLayer::tanshe[i]->Rx,My3DLayer::tanshe[i]->Ry,My3DLayer::tanshe[i]->Rz);
        My3DLayer::tanshe[i]->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                     &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::tanshe[i]->texturename)]));
        MatrixState3D::popMatrix();
    }

    //更新主方块姿态
    ZFTManager::Update_mainbox();
    //绘制主方块
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(My3DLayer::mainBox->x,My3DLayer::mainBox->y,My3DLayer::mainBox->z);
    MatrixState3D::scale(ZFTManager::MainScalex,ZFTManager::MainScaley,ZFTManager::MainScalez);
    MatrixState3D::rotate(My3DLayer::mainBox->Rangle,My3DLayer::mainBox->Rx,My3DLayer::mainBox->Ry,My3DLayer::mainBox->Rz);
    My3DLayer::mainBox->drawSelf(MyVulkanManager::cmdBuffer,MyVulkanManager::sqsCTL->pipelineLayout,MyVulkanManager::sqsCTL->pipeline,
                                 &(MyVulkanManager::sqsCTL->descSet[TextureManager::getVkDescriptorSetIndexForCommonTexLight(My3DLayer::mainBox->texturename)]));
    MatrixState3D::popMatrix();
    //更新检测主方块的存货状态
    if(!TanShe::tanshe_flag){
        DiCiBox::DiCiCheckLife();           //地刺存活判断
        JiaZiBox::JiaZiCheckLife();         //夹子存活判断
        DiaoLuoManager::DiaoLuoCheckLife(); //掉落存活判断
        YiDongBox::YiDongCheckLife();       //移动存活判断
    }

    TanShe::TanSheCheck();                  //弹射判断
    ZFTManager::Win();                      //胜利判断
    //主方块死亡方式
    if(!ZFTManager::life){
        switch (ZFTManager::Diefoncution){
            case 1:{
                ZFTManager::ZFTDiCiDie();//地刺死亡方式
            break;
            }
            case 2:{
                ZFTManager::ZFTJiaZiDie();//夹子死亡方式
                break;
            }
            case 3:{
                ZFTManager::ZFTDiaoLuoDie();//掉落死亡方式
                break;
            }
            case 4:{
                ZFTManager::ZFTYiDongDie();//移动板死亡方式
                break;
            }
        }
    }
    if(My3DLayer::mainBox->y<=-50){                 //重置附属方块
        for(int i=0;i<ATTACH_BOX_NUMBER;i++){
            My3DLayer::attachbox_draw_count[i]=0;
        }
        for(int i=0;i<ATTACH_BOX_NUMBER;i++){
            My3DLayer::attachbox_draw_licence[i]=0;
        }
    }

}

void MyDraw::Draw2Dobject(){
    //绘制2D仪表板物体===================================================begin===
    //保护现场
    if(!Touch2D::Button_start){
        MatrixState2D::pushMatrix();
        MatrixState2D::translate(0.05,0.5,0);
        MatrixState2D::scale(0.6,0.25,0);
        //方块历险记 主界面 名称
        //绘制物体
        MyVulkanManager::d2dA->drawSelf( MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout, MyVulkanManager::sqsD2D->pipeline,
                                         &( MyVulkanManager::sqsD2D->descSet[TextureManager::getVkDescriptorSetIndexForDashboard2D("texture/steps.pkm")]));
        //恢复现场
        MatrixState2D::popMatrix();
        //__________________________________________________________________________________________________________________
        MatrixState2D::pushMatrix();
        MatrixState2D::translate(0,0.25,0);
        MatrixState2D::scale(0.2,0.15,0);
        //点击开始 主界面 名称
        //绘制物体
        MyVulkanManager::d2dA->drawSelf( MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout, MyVulkanManager::sqsD2D->pipeline,
                                         &( MyVulkanManager::sqsD2D->descSet[TextureManager::getVkDescriptorSetIndexForDashboard2D("texture/dianjistart.pkm")]));
        //恢复现场
        MatrixState2D::popMatrix();

        //========================================音效 begin=============================================
        if(Touch2D::Button_mute)
        {   //音效按钮关闭
            MatrixState2D::pushMatrix();
            MatrixState2D::translate(0, -0.5, 0);
            MatrixState2D::scale(0.1,0.1,0);
            MyVulkanManager::d2dA->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout, MyVulkanManager::sqsD2D->pipeline,
                    &(MyVulkanManager::sqsD2D->descSet[TextureManager::getVkDescriptorSetIndexForDashboard2D("texture/musicclosebtn.pkm")]));
            MatrixState2D::popMatrix();
        } else
        {   //音效按钮打开
            MatrixState2D::pushMatrix();
            MatrixState2D::translate(0, -0.5, 0);
            MatrixState2D::scale(0.1,0.1,0);
            MyVulkanManager::d2dA->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout, MyVulkanManager::sqsD2D->pipeline,
                    &(MyVulkanManager::sqsD2D->descSet[TextureManager::getVkDescriptorSetIndexForDashboard2D("texture/musicbtn.pkm")]));
            MatrixState2D::popMatrix();
        }
        //========================================音效 end=============================================

        //______________________________________________________________________________________________________________________

            MatrixState2D::pushMatrix();
            MatrixState2D::translate(-0.4,-0.5,0);
            MatrixState2D::scale(0.1,0.1,0);
             //主界面 关于游戏 按钮
            //绘制物体
            MyVulkanManager::d2dA->drawSelf( MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout, MyVulkanManager::sqsD2D->pipeline,
                                             &( MyVulkanManager::sqsD2D->descSet[TextureManager::getVkDescriptorSetIndexForDashboard2D("texture/about.pkm")]));
            //恢复现场
            MatrixState2D::popMatrix();

        //_________________________________________________________________________________________________________________
        if(!Touch2D::Button_exit){//如果没有点击退出按钮
            MatrixState2D::pushMatrix();
            MatrixState2D::translate(0.4,-0.5,0);
            MatrixState2D::scale(0.1,0.1,0);
            // 主界面 退出 按钮
            //绘制物体
            MyVulkanManager::d2dA->drawSelf( MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout, MyVulkanManager::sqsD2D->pipeline,
                                             &( MyVulkanManager::sqsD2D->descSet[TextureManager::getVkDescriptorSetIndexForDashboard2D("texture/exitbtn.pkm")]));
            //恢复现场
            MatrixState2D::popMatrix();
        }

    }

    if(!ZFTManager::life){
        MatrixState2D::pushMatrix();
        MatrixState2D::translate(-0.11,-0.5,0);
        MatrixState2D::scale(0.1,0.1,0);
        // 主角死亡界面 重新开始 按钮
        //绘制物体
        MyVulkanManager::d2dA->drawSelf( MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout, MyVulkanManager::sqsD2D->pipeline,
                                         &( MyVulkanManager::sqsD2D->descSet[TextureManager::getVkDescriptorSetIndexForDashboard2D("texture/resume.pkm")]));
        //恢复现场
        MatrixState2D::popMatrix();

        MatrixState2D::pushMatrix();
        MatrixState2D::translate(0.11,-0.5,0);
        MatrixState2D::scale(0.1,0.1,0);
        //主角死亡界面 退回主界面 按钮
        //绘制物体
        MyVulkanManager::d2dA->drawSelf( MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout, MyVulkanManager::sqsD2D->pipeline,
                                         &( MyVulkanManager::sqsD2D->descSet[TextureManager::getVkDescriptorSetIndexForDashboard2D("texture/back.pkm")]));
        //恢复现场
        MatrixState2D::popMatrix();
    }

    if(Touch2D::Button_about){
        MatrixState2D::pushMatrix();
        MatrixState2D::translate(0,-0.1,0);
        MatrixState2D::scale(0.4,0.2,0);
        // 点击主界面 关于游戏按钮 弹出的游戏说明文字
        //绘制物体
        MyVulkanManager::d2dA->drawSelf( MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout, MyVulkanManager::sqsD2D->pipeline,
                                         &( MyVulkanManager::sqsD2D->descSet[TextureManager::getVkDescriptorSetIndexForDashboard2D("texture/guanyu.pkm")]));
        //恢复现场
        MatrixState2D::popMatrix();
    }

    //绘制2D仪表板物体===================================================end=====
}