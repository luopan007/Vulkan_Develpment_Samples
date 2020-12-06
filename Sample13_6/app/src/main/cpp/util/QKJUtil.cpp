#include "QKJUtil.h"
#include "EYYCFCUtil.h"
vector<double> QKJUtil::calQKJ(double p0x,
	double p0y,
	double p0z,
	double p1x,
	double p1y,
	double p1z,
	double p2x,
	double p2y,
	double p2z,
	double p0s,
	double p0t,
	double p1s,
	double p1t,
	double p2s,
	double p2t) {
	double a0 = p1s - p0s;
	double b0 = p1t - p0t;
	double c0 = p0x - p1x;
	double a1 = p2s - p0s;
	double b1 = p2t - p0t;
	double c1 = p0x - p2x;
	vector<double> TBX = EYYCFCUtil::solveEquation(a0, b0, c0, a1, b1, c1);
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
	vector<double> tvector;
	tvector.push_back(TBX[0]);
	tvector.push_back(TBY[0]);
	tvector.push_back(TBZ[0]);
	return tvector;
}
