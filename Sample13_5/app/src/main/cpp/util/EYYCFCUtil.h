#ifndef EYYCFCUTIL_H
#define EYYCFCUTIL_H
#include <vector>
using namespace std;
class EYYCFCUtil
{
public:
	static vector<double> solveEquation(
		double a0,
		double b0,
		double c0,
		double a1,
		double b1,
		double c1
	);
};
#endif
