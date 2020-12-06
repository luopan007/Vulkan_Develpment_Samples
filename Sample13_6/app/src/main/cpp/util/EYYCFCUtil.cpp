#include "EYYCFCUtil.h"
#include <vector>
using namespace std;
 vector<double> EYYCFCUtil::solveEquation(
	 double a0,
	 double b0,
	 double c0,
	 double a1,
	 double b1,
	 double c1)
{
	 double x = (c1*b0 - c0*b1) / (a0*b1 - a1*b0);
	 double y = (-a0*x - c0) / b0;
	 vector<double> tvector;
	 tvector.push_back(x);
	 tvector.push_back(y);
	 return tvector;
}
