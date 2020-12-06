#include "AABB.h"
AABB::AABB(float minXIn, float maxXIn, float minYIn, float maxYIn, float minZIn, float maxZIn)
{
	this->minX = minXIn;
	this->maxX = maxXIn;
	this->minY = minYIn;
	this->maxY = maxYIn;
	this->minZ = minZIn;
	this->maxZ = maxZIn;
}
/*
* Woo提出的方法，先判断矩形边界框的哪个面会相交，
* 再检测射线与包含这个面的平面的相交性。
* 如果交点在盒子中，那么射线与矩形边界框相交，
* 否则不存在相交
*/
float AABB::rayIntersect( //计算当前物体AABB 包围盒与拾取射线最近交点t 值的方法
		Vector3f* rayStart, //射线起点A 坐标
		Vector3f* rayEnd){ //射线终点B 坐标
	float t = FLT_MAX; //如果未相交则返回float 型的最大值
	float tempT = (minX - rayStart->x) / (rayEnd->x - rayStart->x);//计算射线与X=min->x 平面交点的t 值
	if (tempT >= 0 && tempT <= 1){ //若t 值在0～1 的范围内
		float y=rayStart->y+tempT*(rayEnd->y-rayStart->y);//计算射线与X=min->x 平面交点的Y 坐标
		float z=rayStart->z+tempT*(rayEnd->z-rayStart->z);//计算射线与X=min->x 平面交点的Z 坐标
		if (y >= minY&&y <= maxY&&z >= minZ&&z <= maxZ){//若交点Y、Z 坐标在包围盒此面范围内
			if (tempT<t){ //若当前交点的t 值小于原先t 值
				t = tempT; //更新t 值
			}}}
	tempT = (maxX - rayStart->x)/(rayEnd->x - rayStart->x); //计算射线与X=max->x 平面交点的t 值
	if (tempT >= 0 && tempT <= 1){ //若t 值在0～1 的范围内
		float y=rayStart->y+tempT*(rayEnd->y-rayStart->y);//计算射线与X=max->x 平面交点的Y 坐标
		float z=rayStart->z+tempT*(rayEnd->z-rayStart->z);//计算射线与X=max->x 平面交点的Z 坐标
		if (y >= minY&&y <= maxY&&z >= minZ&&z <= maxZ){//若交点Y、Z 坐标在包围盒此面范围内
			if (tempT<t){ //若当前交点的t 值小于原先t 值
				t = tempT; //更新t 值
			}}}
	tempT = (minY - rayStart->y) / (rayEnd->y - rayStart->y);
	if (tempT >= 0 && tempT <= 1)
	{
		float x = rayStart->x + tempT*(rayEnd->x - rayStart->x);
		float z = rayStart->z + tempT*(rayEnd->z - rayStart->z);
		if (x >= minX&&x <= maxX&&z >= minZ&&z <= maxZ)
		{
			if (tempT<t)
			{
				t = tempT;
			}
		}
	}
	tempT = (maxY - rayStart->y) / (rayEnd->y - rayStart->y);
	if (tempT >= 0 && tempT <= 1)
	{
		float x = rayStart->x + tempT*(rayEnd->x - rayStart->x);
		float z = rayStart->z + tempT*(rayEnd->z - rayStart->z);
		if (x >= minX&&x <= maxX&&z >= minZ&&z <= maxZ)
		{
			if (tempT<t)
			{
				t = tempT;
			}
		}
	}
	tempT = (minZ - rayStart->z) / (rayEnd->z - rayStart->z);
	if (tempT >= 0 && tempT <= 1)
	{
		float x = rayStart->x + tempT*(rayEnd->x - rayStart->x);
		float y = rayStart->y + tempT*(rayEnd->y - rayStart->y);
		if (x >= minX&&x <= maxX&&y >= minY&&y <= maxY)
		{
			if (tempT<t)
			{
				t = tempT;
			}
		}
	}
	tempT = (maxZ - rayStart->z) / (rayEnd->z - rayStart->z);
	if (tempT >= 0 && tempT <= 1)
	{
		float x = rayStart->x + tempT*(rayEnd->x - rayStart->x);
		float y = rayStart->y + tempT*(rayEnd->y - rayStart->y);
		if (x >= minX&&x <= maxX&&y >= minY&&y <= maxY)
		{
			if (tempT<t)
			{
				t = tempT;
			}
		}
	}
	return t;//返回最近相交点的t 值
}
AABB::~AABB()
{
}