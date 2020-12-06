#include "TouchableObject.h"
AABB* TouchableObject::getCurrBox() { //获取物体AABB 包围盒的方法
	return preBox; //返回指向物体AABB 包围盒对象的指针
}
void TouchableObject::changeOnTouch(bool flag) { //根据选中标志改变物体颜色、尺寸的方法
	if (flag) { //若为选中状态
		color[0] = 0.0f;color[1] = 1.0f;color[2] = 0.0f;color[3] = 1.0f; //改变物体颜色为绿色
		size = 3.0f; //改变物体放大率为选中态
	}else { //若为未选中状态
		color[0] = 1.0f;color[1] = 1.0f;color[2] = 1.0f;color[3] = 1.0f; //恢复物体颜色为白色
		size = 1.5f; //恢复物体放大率为非选中态
	}}
void TouchableObject::copyM() { //复制当前基本变换矩阵的方法
	memcpy(m, MatrixState3D::getMMatrix(),sizeof(float)*16); //拷贝当前基本变换矩阵
}
TouchableObject::~TouchableObject() { //析构函数
	delete[] m;delete[] color; //释放内存
}