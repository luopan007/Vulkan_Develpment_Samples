#include "LandData.h"
#include "stdio.h"
#include <math.h>
#include "map"
#include "vector"
#include "Normal.h"
using namespace std;
#define LAND_SPAN 1									
#define LAND_HIGHEST 198							
#define LAND_ADJUST_Y 0								
LandData::LandData(int width, int height, unsigned char* data)
{
	this->width = width;
	this->height = height;
	gdz= new float*[height];
	for (int i = 0; i<height; ++i) gdz[i] = new float[width];
	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			int h = data[i*width + j];
			gdz[i][j] = h*LAND_HIGHEST / 255.0f + LAND_ADJUST_Y;
		}
	}
	rows = height - 1;
	cols = width - 1;
	gzCount = rows*cols;
	vCount = gzCount * 2 * 3;
	vData = new float[vCount * 8];
	xStart = -LAND_SPAN*cols / 2.0f;
	zStart = -LAND_SPAN*rows / 2.0f;
	vNormal = new float[3]();
	calNormals();
	float sSpan = 1.0f / cols;
	float tSpan = 1.0f / rows;
	int indexTemp = 0;
	for (int i = 0; i<rows; i++)
	{
		for (int j = 0; j<cols; j++)
		{
			float s0 = sSpan*j;
			float t0 = tSpan*i;
			float s1 = sSpan*(j + 1);
			float t1 = tSpan*i;
			float s2 = sSpan*j;
			float t2 = tSpan*(i + 1);
			float s3 = sSpan*(j + 1);
			float t3 = tSpan*(i + 1);
			vData[indexTemp++] = vertices[(i*width + j) * 3 + 0];
			vData[indexTemp++] = vertices[(i*width + j) * 3 + 1];
			vData[indexTemp++] = vertices[(i*width + j) * 3 + 2];
			vData[indexTemp++] = s0;
			vData[indexTemp++] = t0;
			vData[indexTemp++] = normals[(i*width + j) * 3 + 0];
			vData[indexTemp++] = normals[(i*width + j) * 3 + 1];
			vData[indexTemp++] = normals[(i*width + j) * 3 + 2];
			vData[indexTemp++] = vertices[((i + 1)*width + j + 1) * 3 + 0];
			vData[indexTemp++] = vertices[((i + 1)*width + j + 1) * 3 + 1];
			vData[indexTemp++] = vertices[((i + 1)*width + j + 1) * 3 + 2];
			vData[indexTemp++] = s3;
			vData[indexTemp++] = t3;
			vData[indexTemp++] = normals[((i + 1)*width + j + 1) * 3 + 0];
			vData[indexTemp++] = normals[((i + 1)*width + j + 1) * 3 + 1];
			vData[indexTemp++] = normals[((i + 1)*width + j + 1) * 3 + 2];
			vData[indexTemp++] = vertices[(i*width + j + 1) * 3 + 0];
			vData[indexTemp++] = vertices[(i*width + j + 1) * 3 + 1];
			vData[indexTemp++] = vertices[(i*width + j + 1) * 3 + 2];
			vData[indexTemp++] = s1;
			vData[indexTemp++] = t1;
			vData[indexTemp++] = normals[(i*width + j +1) * 3 + 0];
			vData[indexTemp++] = normals[(i*width + j +1) * 3 + 1];
			vData[indexTemp++] = normals[(i*width + j +1) * 3 + 2];
			vData[indexTemp++] = vertices[(i*width + j) * 3 + 0];
			vData[indexTemp++] = vertices[(i*width + j) * 3 + 1];
			vData[indexTemp++] = vertices[(i*width + j) * 3 + 2];
			vData[indexTemp++] = s0;
			vData[indexTemp++] = t0;
			vData[indexTemp++] = normals[(i*width +j) * 3 + 0];
			vData[indexTemp++] = normals[(i*width +j) * 3 + 1];
			vData[indexTemp++] = normals[(i*width +j) * 3 + 2];
			vData[indexTemp++] = vertices[((i + 1)*width + j) * 3 + 0];
			vData[indexTemp++] = vertices[((i + 1)*width + j) * 3 + 1];
			vData[indexTemp++] = vertices[((i + 1)*width + j) * 3 + 2];
			vData[indexTemp++] = s2;
			vData[indexTemp++] = t2;
			vData[indexTemp++] = normals[((i+1)*width + j) * 3 + 0];
			vData[indexTemp++] = normals[((i+1)*width + j) * 3 + 1];
			vData[indexTemp++] = normals[((i+1)*width + j) * 3 + 2];
			vData[indexTemp++] = vertices[((i + 1)*width + j+1) * 3 + 0];
			vData[indexTemp++] = vertices[((i + 1)*width + j+1) * 3 + 1];
			vData[indexTemp++] = vertices[((i + 1)*width + j+1) * 3 + 2];
			vData[indexTemp++] = s3;
			vData[indexTemp++] = t3;
			vData[indexTemp++] = normals[((i + 1)*width + j + 1) * 3 + 0];
			vData[indexTemp++] = normals[((i + 1)*width + j + 1) * 3 + 1];
			vData[indexTemp++] = normals[((i + 1)*width + j + 1) * 3 + 2];
		}
	}
}
void getCrossProduct(float x1, float y1, float z1, float x2, float y2, float z2,float* normal) {
	float A = y1 * z2 - y2 * z1;
	float B = z1 * x2 - z2 * x1;
	float C = x1 * y2 - x2 * y1;
	normal[0] = A;
	normal[1] = B;
	normal[2] = C;
}
void LandData::vectorNormal(float* normal) {
	float module = (float)sqrt((double)(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]));
	normal[0] /= module;
	normal[1] /= module;
	normal[2] /= module;
}
void LandData::calNormals()
{
	vertices = new float[width*height * 3]();
	normals = new float[width*height * 3]();
	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			vertices[(i*width + j) * 3 + 0] = xStart + j*LAND_SPAN;
			vertices[(i*width + j) * 3 + 1] = gdz[i][j];
			vertices[(i*width + j) * 3 + 2] = zStart + i*LAND_SPAN; 
		}
	}
	int* index = new int[4]();
	for (int i = 0; i<rows; i++)
	{
		for (int j = 0; j<cols; j++)
		{
			index[0] = i*width + j;
			index[1] = index[0] + 1;
			index[2] = index[0] + width;
			index[3] = index[2] + 1;
			float vxa = vertices[index[3] * 3 + 0] - vertices[index[0] * 3 + 0];
			float vya = vertices[index[3] * 3 + 1] - vertices[index[0] * 3 + 1];
			float vza = vertices[index[3] * 3 + 2] - vertices[index[0] * 3 + 2];
			float vxb = vertices[index[1] * 3 + 0] - vertices[index[0] * 3 + 0];
			float vyb = vertices[index[1] * 3 + 1] - vertices[index[0] * 3 + 1];
			float vzb = vertices[index[1] * 3 + 2] - vertices[index[0] * 3 + 2];
			getCrossProduct(vxa, vya, vza, vxb, vyb, vzb, vNormal);
			vectorNormal(vNormal);
			for (int k = 0; k<4; k++) {
				if (k == 2)continue;
				normals[index[k] * 3 + 0] += vNormal[0];
				normals[index[k] * 3 + 1] += vNormal[1];
				normals[index[k] * 3 + 2] += vNormal[2];
			}
			vxa = vertices[index[2] * 3 + 0] - vertices[index[0] * 3 + 0];
			vya = vertices[index[2] * 3 + 1] - vertices[index[0] * 3 + 1];
			vza = vertices[index[2] * 3 + 2] - vertices[index[0] * 3 + 2];
			vxb = vertices[index[3] * 3 + 0] - vertices[index[0] * 3 + 0];
			vyb = vertices[index[3] * 3 + 1] - vertices[index[0] * 3 + 1];
			vzb = vertices[index[3] * 3 + 2] - vertices[index[0] * 3 + 2];
			getCrossProduct(vxa, vya, vza, vxb, vyb, vzb, vNormal);
			vectorNormal(vNormal);
			for (int k = 0; k<4; k++) {
				if (k == 1)continue;
				normals[index[k]* 3 + 0] += vNormal[0];
				normals[index[k]* 3 + 1] += vNormal[1];
				normals[index[k]* 3 + 2] += vNormal[2];
			}
		}
	}
}
