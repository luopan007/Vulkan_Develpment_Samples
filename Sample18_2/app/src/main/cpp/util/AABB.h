#pragma once
#ifndef AABB_H
#define AABB_H
#include "Vector3f.h"
#include <string>
class AABB
{
public:
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;
	AABB(float minXIn, float maxXIn, float minYIn, float maxYIn, float minZIn, float maxZIn);
	~AABB();
	float rayIntersect(
			Vector3f* rayStart,
			Vector3f* rayEnd
	);
};
#endif