#ifndef VULKANEXBASE_LAND_H
#define VULKANEXBASE_LAND_H
class LandData
{
public:
	float* vData;
	int vCount;
	int rows;
	int cols;
	int gzCount;
	float xStart;
	float zStart;
	float* vNormal;
	void calNormals();
	static void vectorNormal(float* vector);
	float* normals;
	int width;
	int height; 
	float **gdz;
	float *vertices;
	LandData(int width, int height, unsigned char* data);
};
#endif
