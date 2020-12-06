#pragma once
#ifndef BallForControl_h
#define BallForControl_h
#include "Quaternion.h"
#include "Vector3f.h"
#include "math.h"
#include "ObjObject.h"
#include <vector>
#include "MatrixState3D.h"
#include "ShaderQueueSuit_Common.h"
#include "TextureManager.h"
using  namespace std;
#define TIME_SPAN 0.05f //球移动每一步的模拟时间间隔(此值越小模拟效果越真实，但计算量增大)
#define V_TENUATION 0.995f //速度衰减系数
#define V_THRESHOLD 0.1f //速度阈值，小于此阈值的速度认为是0
#define B_MODEL_SIZE 1.0f //球模型半径
#define BALL_SCALE 0.48f //球缩放比例
#define BALL_R (B_MODEL_SIZE*BALL_SCALE) //球绘制半径
#define BANISTERS_HEIGHT 1.0f //桌面挡板高度
#define TABLE_SIZE 6.0f //桌面边长(本案例中桌面为正方形)
class BallForControl {
public:
	ObjObject* obj; //指向用于绘制的桌球对象的指针
	float xOffset; //桌球的X 位置
	float zOffset; //桌球的Z 位置
	string textureName; //桌球纹理的名称
	float vx = 0.0f; //桌球的X 轴向速度
	float vz = 0.0f; //桌球的Z 轴向速度
	Quaternion* quaternionTotal = Quaternion::getIdentityQuaternion();//记录桌球旋转总姿态的四元数
	float angleCurr = 0; //当前桌球的旋转总角度(绘制用)
	float currAxisX; //当前桌球总旋转轴向量的X 分量(绘制用)
	float currAxisZ; //当前桌球总旋转轴向量的Z 分量(绘制用)
	float currAxisY=0;//当前桌球总旋转轴向量的Y 分量(绘制用，因为球在桌面上运动旋转轴平行于桌面)
	static Vector3f* tmpAxis; //指向临时存放桌球一步旋转轴向量对象的指针
	static Quaternion* tmpQuaternion; //指向临时存放桌球一步旋转信息的四元数对象的指针
	static Vector3f* axis; //指向临时存储桌球当前总旋转轴向量对象的指针
	BallForControl(ObjObject* btv, float xOffset, float zOffset, string textureName); //构造函数
	~BallForControl(); //析构函数
	void drawSelf(VkCommandBuffer& cmd, ShaderQueueSuit_Common* sqsCL); //绘制方法
	void go(); //根据桌球当前速度向前运动一步的方法
};
#endif // !BallForControl_h

