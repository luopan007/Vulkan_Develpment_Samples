#include "AABB.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

AABB::AABB(float minXIn,float maxXIn,float minYIn,
		   float maxYIn,float minZIn,float maxZIn){ //构造函数
	minX=minXIn;maxX=maxXIn;minY=minYIn; //记录坐标最小值
	maxY=maxYIn;minZ=minZIn;maxZ=maxZIn; //记录坐标最大值
}
AABB::~AABB() { }; //析构函数


