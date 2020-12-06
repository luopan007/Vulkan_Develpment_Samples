#pragma once
#ifndef CollisionUtil_h
#define CollisionUtil_h
#include "BallForControl.h"
#include <math.h>
class CollisionUtil {
public :
	static float dotProduct(float x1, float y1, float z1, float x2, float y2, float z2)
	{
		return
				x1 * x2 +
				y1 * y2 +
				z1 * z2;
	}
	static float mould(float x,float y,float z)
	{
		return (float)sqrt(x * x + y * y + z * z);
	}
	static float angle(float x1, float y1, float z1, float x2, float y2, float z2)
	{
		float dp = dotProduct(x1, y1, z1, x2, y2, z2);
		float m1 = mould(x1, y1, z1);
		float m2 = mould(x2, y2, z2);
		float aCos = dp / (m1*m2);
		if (aCos>1)
		{
			aCos = 1;
		}
		else if (aCos<-1)
		{
			aCos = -1;
		}
		return (float)acos(aCos);
	}
    static bool collisionCalculate(BallForControl* balla, BallForControl* ballb){ //计算两球物理碰撞的方法
        float BAx = balla->xOffset - ballb->xOffset; //求球A 与球B 的X 坐标差
        float BAz = balla->zOffset - ballb->zOffset; //求球A 与球B 的Z 坐标差
        float mvBA = mould(BAx, 0, BAz); //求AB 球中心连线向量BA 的模
        if (mvBA >= BALL_R * 2){ return false;} //若两球距离大于球直径则没有碰撞
        float vB = (float)sqrt(ballb->vx*ballb->vx + ballb->vz*ballb->vz); //求B 球的速度大小(标量值)
        float vbCollx = 0;float vbCollz = 0; //用于记录球B 平行于向量BA 方向速度分量的XZ 分量
        float vbVerticalX = 0;float vbVerticalZ = 0; //用于记录球B 垂直于向量BA 方向速度分量的XZ 分量
        if (V_THRESHOLD<vB){ //若球B 速度大于阈值，则进行下面的分解计算
            float bAngle = angle( //求球B 的速度向量与BA 向量的夹角(以弧度计)
                    ballb->vx, 0, ballb->vz,BAx, 0, BAz);
            float vbColl = vB*(float)cos(bAngle); //求B 球在碰撞方向上的速度大小
            vbCollx = (vbColl / mvBA)*BAx; //求B 球在碰撞方向上的速度X 分量
            vbCollz = (vbColl / mvBA)*BAz; //求B 球在碰撞方向上的速度Z 分量
            vbVerticalX = ballb->vx - vbCollx; //求B 球在碰撞垂直方向上的速度X 分量
            vbVerticalZ = ballb->vz - vbCollz; //求B 球在碰撞垂直方向上的速度Z 分量
        }
		float vA = (float)sqrt(balla->vx*balla->vx + balla->vz*balla->vz);
		float vaCollx = 0;
		float vaCollz = 0;
		float vaVerticalX = 0;
		float vaVerticalZ = 0;
		if (V_THRESHOLD<vA){
			float aAngle = angle
					(
							balla->vx, 0, balla->vz,
							BAx, 0, BAz
					);
			float vaColl = vA*(float)cos(aAngle);
			vaCollx = (vaColl / mvBA)*BAx;
			vaCollz = (vaColl / mvBA)*BAz;
			vaVerticalX = balla->vx - vaCollx;
			vaVerticalZ = balla->vz - vaCollz;
		}
        balla->vx = vaVerticalX + vbCollx; //设置碰撞后A 球X 方向速度分量
        balla->vz = vaVerticalZ + vbCollz; //设置碰撞后A 球Z 方向速度分量
        ballb->vx = vbVerticalX + vaCollx; //设置碰撞后B 球X 方向速度分量
        ballb->vz = vbVerticalZ + vaCollz; //设置碰撞后B 球Z 方向速度分量
        return true;
	}
};
#endif
