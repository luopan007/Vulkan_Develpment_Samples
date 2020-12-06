#ifndef MS3DTRIANGLE_H_
#define MS3DTRIANGLE_H_
#include "../element/Vector3f.h"
#include <vector>
using namespace std;
class MS3DTriangle{
private:
	int* indexs; //指向三角形三个顶点索引值数组首地址的指针
	Vector3f* s; //指向存储三角形三个顶点纹理S 坐标的复合数对象的指针
	Vector3f* t; //指向存储三角形三个顶点纹理T 坐标的复合数对象的指针
	int smoothingGroup; //三角形面所处的平滑组编号
	int groupIndex; //三角形面所处的组索引
public:
	MS3DTriangle(unsigned char* binaryData,int* binaryData_index); //构造函数
	~MS3DTriangle(); //析构函数
	vector<Vector3f*> normals; //三角形三个顶点法向量的列表
	int* getIndexs(); //获取三角形面三个顶点索引的方法
	Vector3f* getS(); //获取三角形面三个顶点纹理S 坐标的方法
	Vector3f* getT(); //获取三角形面三个顶点纹理T 坐标的方法
	int getSmoothingGroup(); //获取三角形面所处的平滑组编号的方法
	int getGroupIndex(); //获取三角形面所处的组索引的方法
};
#endif
