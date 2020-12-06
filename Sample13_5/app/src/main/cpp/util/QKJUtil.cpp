#include "QKJUtil.h"
#include "EYYCFCUtil.h"
vector<double> QKJUtil::calQKJ( //计算切向量的方法
	double p0x, //三角形面第1 个顶点的x 坐标
	double p0y, //三角形面第1 个顶点的y 坐标
	double p0z, //三角形面第1 个顶点的z 坐标
	double p1x, //三角形面第2 个顶点的z 坐标
	double p1y, //三角形面第2 个顶点的y 坐标
	double p1z, //三角形面第2 个顶点的x 坐标
	double p2x, //三角形面第3 个顶点的x 坐标
	double p2y, //三角形面第3 个顶点的y 坐标
	double p2z, //三角形面第3 个顶点的z 坐标
	double p0s, //三角形面第1 个顶点的s 纹理坐标
	double p0t, //三角形面第1 个顶点的t 纹理坐标
	double p1s, //三角形面第2 个顶点的s 纹理坐标
	double p1t, //三角形面第2 个顶点的t 纹理坐标
	double p2s, //三角形面第3 个顶点的s 纹理坐标
	double p2t) { //三角形面第3 个顶点的t 纹理坐标
	double a0 = p1s - p0s; //顶点1 与顶点0 纹理坐标s 差值
	double b0 = p1t - p0t; //顶点1 与顶点0 纹理坐标t 差值
	double c0 = p0x - p1x; //顶点1 与顶点0 位置坐标x 差值
	double a1 = p2s - p0s; //顶点2 与顶点0 位置坐标s 差值
	double b1 = p2t - p0t; //顶点2 与顶点0 纹理坐标t 差值
	double c1 = p0x - p2x; //顶点2 与顶点0 位置坐标x 差值
	vector<double> TBX = EYYCFCUtil::solveEquation(a0, b0, c0, a1, b1, c1);//解二元一次方程组获得切向量
	a0 = p1s - p0s;
	b0 = p1t - p0t;
	c0 = p0y - p1y;
	a1 = p2s - p0s;
	b1 = p2t - p0t;
	c1 = p0y - p2y;
	vector<double> TBY = EYYCFCUtil::solveEquation(a0, b0, c0, a1, b1, c1);
	a0 = p1s - p0s;
	b0 = p1t - p0t;
	c0 = p0z - p1z;
	a1 = p2s - p0s;
	b1 = p2t - p0t;
	c1 = p0z - p2z;
	vector<double> TBZ = EYYCFCUtil::solveEquation(a0, b0, c0, a1, b1, c1);
	vector<double> tvector;//存放切向量的列表
	tvector.push_back(TBX[0]);//将切向量的X 分量存入列表
	tvector.push_back(TBY[0]);//将切向量的Y 分量存入列表
	tvector.push_back(TBZ[0]);//将切向量的Z 分量存入列表
	return tvector;//返回列表
}
