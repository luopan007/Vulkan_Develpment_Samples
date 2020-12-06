#include "SZTJCUtil.h"
#include <math.h>

using namespace std;

//根据投影、摄像机及基本变换总矩阵求出视锥体在物体坐标系中的平面方程
void SZTJCUtil::calculateFrustumPlanes(float* mvp,float* g_frustumPlanes)
{
	float t; //声明规格化辅助变量
	g_frustumPlanes[0*4+0] = mvp[3] - mvp[0]; //计算平面方程系数A
	g_frustumPlanes[0*4+1] = mvp[7] - mvp[4]; //计算平面方程系数B
	g_frustumPlanes[0*4+2] = mvp[11] - mvp[8]; //计算平面方程系数C
	g_frustumPlanes[0*4+3] = mvp[15] - mvp[12]; //计算平面方程系数D

	t = (float) sqrt( g_frustumPlanes[0*4+0] * g_frustumPlanes[0*4+0] +
					  g_frustumPlanes[0*4+1] * g_frustumPlanes[0*4+1] +
					  g_frustumPlanes[0*4+2] * g_frustumPlanes[0*4+2] ); //计算规格化辅助变量值
	g_frustumPlanes[0*4+0] /= t; //规格化平面方程系数A
	g_frustumPlanes[0*4+1] /= t; //规格化平面方程系数B
	g_frustumPlanes[0*4+2] /= t; //规格化平面方程系数C
	g_frustumPlanes[0*4+3] /= t; //规格化平面方程系数D

	//求出视锥体左面的平面方程并规格化
	 g_frustumPlanes[1*4+0] = mvp[ 3] + mvp[ 0];
	 g_frustumPlanes[1*4+1] = mvp[ 7] + mvp[ 4];
	 g_frustumPlanes[1*4+2] = mvp[11] + mvp[ 8];
	 g_frustumPlanes[1*4+3] = mvp[15] + mvp[12];
	 t = (float) sqrt( g_frustumPlanes[1*4+0] * g_frustumPlanes[1*4+0] +
	 g_frustumPlanes[1*4+1] * g_frustumPlanes[1*4+1] +
	 g_frustumPlanes[1*4+2] * g_frustumPlanes[1*4+2] );
	 g_frustumPlanes[1*4+0] /= t;
	 g_frustumPlanes[1*4+1] /= t;
	 g_frustumPlanes[1*4+2] /= t;
	 g_frustumPlanes[1*4+3] /= t;

	 //求出视锥体底面的平面方程并规格化
	 g_frustumPlanes[2*4+0] = mvp[ 3] + mvp[ 1];
	 g_frustumPlanes[2*4+1] = mvp[ 7] + mvp[ 5];
	 g_frustumPlanes[2*4+2] = mvp[11] + mvp[ 9];
	 g_frustumPlanes[2*4+3] = mvp[15] + mvp[13];

	 t = (float) sqrt( g_frustumPlanes[2*4+0] * g_frustumPlanes[2*4+0] +
	 g_frustumPlanes[2*4+1] * g_frustumPlanes[2*4+1] +
	 g_frustumPlanes[2*4+2] * g_frustumPlanes[2*4+2] );

	 g_frustumPlanes[2*4+0] /= t;
	 g_frustumPlanes[2*4+1] /= t;
	 g_frustumPlanes[2*4+2] /= t;
	 g_frustumPlanes[2*4+3] /= t;

	 //求出视锥体顶面的平面方程并规格化
	 g_frustumPlanes[3*4+0] = mvp[ 3] - mvp[ 1];
	 g_frustumPlanes[3*4+1] = mvp[ 7] - mvp[ 5];
	 g_frustumPlanes[3*4+2] = mvp[11] - mvp[ 9];
	 g_frustumPlanes[3*4+3] = mvp[15] - mvp[13];
	 t = (float) sqrt( g_frustumPlanes[3*4+0] * g_frustumPlanes[3*4+0] +
	 g_frustumPlanes[3*4+1] * g_frustumPlanes[3*4+1] +
	 g_frustumPlanes[3*4+2] * g_frustumPlanes[3*4+2] );
	 g_frustumPlanes[3*4+0] /= t;
	 g_frustumPlanes[3*4+1] /= t;
	 g_frustumPlanes[3*4+2] /= t;
	 g_frustumPlanes[3*4+3] /= t;

	 //求出视锥体远面的平面方程并规格化
	 g_frustumPlanes[4*4+0] = mvp[ 3] - mvp[ 2];
	 g_frustumPlanes[4*4+1] = mvp[ 7] - mvp[ 6];
	 g_frustumPlanes[4*4+2] = mvp[11] - mvp[10];
	 g_frustumPlanes[4*4+3] = mvp[15] - mvp[14];
	 t = (float) sqrt( g_frustumPlanes[4*4+0] * g_frustumPlanes[4*4+0] +
	 g_frustumPlanes[4*4+1] * g_frustumPlanes[4*4+1] +
	 g_frustumPlanes[4*4+2] * g_frustumPlanes[4*4+2] );
	 g_frustumPlanes[4*4+0] /= t;
	 g_frustumPlanes[4*4+1] /= t;
	 g_frustumPlanes[4*4+2] /= t;
	 g_frustumPlanes[4*4+3] /= t;

	 //求出视锥体近面的平面方程并规格化
	 g_frustumPlanes[5*4+0] = mvp[ 3] + mvp[ 2];
	 g_frustumPlanes[5*4+1] = mvp[ 7] + mvp[ 6];
	 g_frustumPlanes[5*4+2] = mvp[11] + mvp[10];
	 g_frustumPlanes[5*4+3] = mvp[15] + mvp[14];

	 t = (float) sqrt( g_frustumPlanes[5*4+0] * g_frustumPlanes[5*4+0] +
	 g_frustumPlanes[5*4+1] * g_frustumPlanes[5*4+1] +
	 g_frustumPlanes[5*4+2] * g_frustumPlanes[5*4+2] );

	 g_frustumPlanes[5*4+0] /= t;
	 g_frustumPlanes[5*4+1] /= t;
	 g_frustumPlanes[5*4+2] /= t;
	 g_frustumPlanes[5*4+3] /= t;
}

bool SZTJCUtil::isPointInHalfSpace(float x,float y,float z,float a,float b,float c,float d){
	float value=a * x +b * y +c * z +d; //将顶点坐标值代入对应平面方程
	if(value<0){ return false; } //顶点在半空间外
	else{ return true; } //顶点在半空间内
}


bool SZTJCUtil::isAllOutHalfSpace(AABB* aabb,float a,float b,float c,float d){
	if(isPointInHalfSpace(aabb->minX,aabb->minY,aabb->minZ,a,b,c,d)){//判断AABB 包围盒的第1 个点
		return false;
	}
	//AABB的第2个点
	if(isPointInHalfSpace(aabb->minX,aabb->minY,aabb->maxZ,a,b,c,d))
	{
		return false;
	}
	//AABB的第3个点
	if(isPointInHalfSpace(aabb->minX,aabb->maxY,aabb->minZ,a,b,c,d))
	{
		return false;
	}
	//AABB的第4个点
	if(isPointInHalfSpace(aabb->minX,aabb->maxY,aabb->maxZ,a,b,c,d))
	{
		return false;
	}
	//AABB的第5个点
	if(isPointInHalfSpace(aabb->maxX,aabb->minY,aabb->minZ,a,b,c,d))
	{
		return false;
	}
	//AABB的第6个点
	if(isPointInHalfSpace(aabb->maxX,aabb->minY,aabb->maxZ,a,b,c,d))
	{
		return false;
	}
	//AABB的第7个点
	if(isPointInHalfSpace(aabb->maxX,aabb->maxY,aabb->minZ,a,b,c,d))
	{
		return false;
	}
	//AABB的第8个点
	if(isPointInHalfSpace(aabb->maxX,aabb->maxY,aabb->maxZ,a,b,c,d))
	{
		return false;
	}
	return true;
}

bool SZTJCUtil::isAABBInSZT(float* mvpMatrix,AABB* aabb){
	float g_frustumPlanes[24]; //视锥体6 个平面方程的ABCD 系数（Ax+By+Cz+D=0）
	calculateFrustumPlanes(mvpMatrix,g_frustumPlanes); //计算视锥体6 个平面方程的ABCD 系数
	for( int i = 0; i < 6; ++i ){ //遍历视锥体的6 个平面
		if(isAllOutHalfSpace( //判断指定AABB 包围盒是否在当前平面确定的半空间之外
				aabb, //指定AABB 包围盒
				g_frustumPlanes[i*4+0], //平面方程系数A
				g_frustumPlanes[i*4+1], //平面方程系数B
				g_frustumPlanes[i*4+2], //平面方程系数C
				g_frustumPlanes[i*4+3] //平面方程系数D
		)){
			return false; //AABB 包围盒在视锥体外
		}}
	return true; //AABB 包围盒在视锥体内
}

