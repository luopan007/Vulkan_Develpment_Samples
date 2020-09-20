#ifndef NORMAL_H
#define NORMAL_H
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
using namespace std;
class Normal{ //代表法向量的类，此类的一个对象表示一个法向量
public:
	float nx; float ny; float nz; //法向量在X、Y、Z 方向的分量
	static bool exist(Normal* normal,set<Normal*> sn); //判断一个向量是否在指定集合中的方法
	static float *getAverage(set<Normal*> sn); //求平均向量的方法
	Normal(float nx,float ny,float nz); //构造函数
	~Normal(); //析构函数
};
#endif
