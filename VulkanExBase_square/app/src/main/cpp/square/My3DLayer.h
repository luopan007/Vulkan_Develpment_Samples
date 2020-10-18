#ifndef VULKANEXBASE_MY3DLAYER_H
#define VULKANEXBASE_MY3DLAYER_H

#include "../util/ObjObject.h"
#define ATTACH_BOX_NUMBER 50//附属方块个数
#define TREE_NUMBER 50	//地图块数
#define DICI_NUMBER 7	//地刺个数
#define JIAZI_NUMBER 5	//夹子个数
#define TANSHE_NUMBER 1 //弹射个数
#define DIAOLUO_NUMBER 4//掉落个数
#define YIDONG_NUMBER 1//移动个数
#define QIZIGUN_NUMBER 2//旗子棍
#define Goorientationright 0
#define Goorientationstright 1
#define Goorientationleft 2
#define Goorientationback 3
#define Goorientationinit 4

class My3DLayer{
public:
    static int  flag[TREE_NUMBER] ;			    //关卡地板布局方式
    static int  flagGo[TREE_NUMBER];            //关卡翻滚方式
    static int  flag1[TREE_NUMBER] ;			    //关卡1地板布局方式
    static int  flag1Go[TREE_NUMBER];            //关卡1翻滚方式
    static int  flag2[TREE_NUMBER];             //关卡2地板布局方式
    static int  flag2Go[TREE_NUMBER];           //关卡2翻滚方式
    static int dicipos[DICI_NUMBER] ;	        //关卡1地刺位置数组
    static int tanshepos[TANSHE_NUMBER];        // 弹射位置
    static int jiazipos[JIAZI_NUMBER] ;			    //关卡1夹子位置数组
    static int diaoluopos[DIAOLUO_NUMBER];			    //关卡1掉落块位置数组
    static int yidongpos[YIDONG_NUMBER];            //关卡1移动位置
    static int tanshepos1[TANSHE_NUMBER];        // 弹射位置
    static int dicipos1[DICI_NUMBER] ;	                 //关卡2地刺位置数组
    static int jiazipos1[JIAZI_NUMBER] ;			       //关卡2夹子位置数组
    static int diaoluopos1[DIAOLUO_NUMBER];			    //关卡2掉落块位置数组
    static int yidongpos1[YIDONG_NUMBER];                //关卡2移动位置
    static int dicipos2[DICI_NUMBER] ;	                 //关卡2地刺位置数组
    static int jiazipos2[JIAZI_NUMBER] ;			       //关卡2夹子位置数组
    static int diaoluopos2[DIAOLUO_NUMBER];			    //关卡2掉落块位置数组
    static int yidongpos2[YIDONG_NUMBER];                //关卡2移动位置
    static int tanshepos2[TANSHE_NUMBER];        // 弹射位置

    static int  attachbox_draw_count[ATTACH_BOX_NUMBER];                          //附属方块绘制计时
    static int  BoxPos;                                        //方块位置记录计时
    static bool BoxPos_licence;                           //方块位置记录开关
    static bool attachbox_draw_licence[ATTACH_BOX_NUMBER];                   //附属小方块绘制开关
    static int  Level;
    static ObjObject* sp3Tree[TREE_NUMBER];			 //底板长方体集合
    static ObjObject* sp3Dici[DICI_NUMBER];			//地刺集合`
    static ObjObject* sp3JiaziLeft[JIAZI_NUMBER];	//左夹子集合
    static ObjObject* sp3JiaziRight[JIAZI_NUMBER];	//右夹子集合
    static ObjObject* sp3JiaziLeftClose[JIAZI_NUMBER];	//左夹子齿轮集合
    static ObjObject* sp3JiaziRightClose[JIAZI_NUMBER];	//右夹子齿轮集合
    static ObjObject* sp3DiaoLuo[DIAOLUO_NUMBER];	//掉落集合
    static ObjObject* sp3DiaoLuoshadow[DIAOLUO_NUMBER];	//掉落阴影集合
    static ObjObject* sp3YiDong[YIDONG_NUMBER];	//移动集合
    static ObjObject* planeForDraw;                 //地面
    static ObjObject* mainBox;                      //主方块
    static ObjObject* attachBox[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox2[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox3[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox4[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox5[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox6[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox7[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox8[ATTACH_BOX_NUMBER];
    static ObjObject* tanshe[TANSHE_NUMBER];
    static ObjObject* qizi;
    static ObjObject* qizigun[QIZIGUN_NUMBER];
    static void initLevel();
    static void NextLevel();
    static void RepeatLevel();
};
#endif //VULKANEXBASE_MY3DLAYER_H
