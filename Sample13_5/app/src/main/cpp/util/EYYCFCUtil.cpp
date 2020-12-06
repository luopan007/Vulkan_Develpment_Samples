#include "EYYCFCUtil.h"
#include <vector>
using namespace std;
vector<double> EYYCFCUtil::solveEquation( //解二元一次方程组的方法
		double a0, //方程0 的x 系数
		double b0, //方程0 的y 系数
		double c0, //方程0 的常数
		double a1, //方程1 的x 系数
		double b1, //方程1 的y 系数
		double c1){ //方程1 的常数
	double x = (c1*b0 - c0*b1) / (a0*b1 - a1*b0); //计算得出的x 值
	double y = (-a0*x - c0) / b0; //计算得出的y 值
	vector<double> tvector; //存放结果的列表
	tvector.push_back(x); //将计算出的x 值存入列表
	tvector.push_back(y); //将计算出的y 值存入列表
	return tvector; //返回结果列表
}

