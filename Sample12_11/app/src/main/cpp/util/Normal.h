#ifndef NORMAL_H
#define NORMAL_H
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
using namespace std;
class Normal
{
public:
	float nx;
	float ny;
	float nz;
	static bool exist(Normal* normal,set<Normal*> sn);
	static float *getAverage(set<Normal*> sn);
	Normal(float nx,float ny,float nz);
	~Normal();
};
#endif
