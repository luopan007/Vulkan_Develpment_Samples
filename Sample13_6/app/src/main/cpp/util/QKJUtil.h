#ifndef QKJUTIL_H
#define QKJUTIL_H
#include <vector>
using namespace std;
class QKJUtil
{
public:
	static vector<double> calQKJ
	(
		double p0x,
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
		double p2t
	);
};

#endif // !QKJUTIL_H


