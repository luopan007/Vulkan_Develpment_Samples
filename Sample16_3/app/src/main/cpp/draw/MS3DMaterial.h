/*
 * MS3DMaterial.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#ifndef MS3DMATERIAL_H_
#define MS3DMATERIAL_H_
#include <string>
using namespace std;
class MS3DMaterial{
private:
	string name; //材质名称
	string format(string path); //用于从文件路径中摘取出纹理图文件名的方法
public:
	MS3DMaterial(unsigned char* binaryData,int* binaryData_index);//构造函数
	~MS3DMaterial(); //析构函数
	float* ambient_color; //指向环境光数据数组首地址的指针
	float* diffuse_color; //指向散射光数据数组首地址的指针
	float* specular_color; //指向镜面光数据数组首地址的指针
	float* emissive_color; //指向自发光数据数组首地址的指针
	float shininess; //粗糙度 0~128
	float transparency; //透明度 0~1
	string textureName; //摘取出的纹理图文件名称
	string getName(); //获取材质名称的方法
};
#endif
