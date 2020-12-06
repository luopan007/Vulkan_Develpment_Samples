/*
 * MS3DMaterial.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DMaterial.h"
#include "MS3DModel.h"
#include <../util/FileUtil.h>
#include <../util/TextureManager.h>
#include <../util/HelpFunction.h>
using namespace std;
MS3DMaterial::MS3DMaterial(unsigned char* binaryData,int* binaryData_index){
	name = FileUtil::myReadString(binaryData,binaryData_index,32); //读取材质的名称
	ambient_color=new float[4]; //创建用于存储环境光数据的数组
	for(int j=0; j<4; j++){ //循环获取环境光4 个色彩通道的值
		ambient_color[j]= //读取并记录环境光的每个色彩通道值
				FileUtil::myReadFloat(binaryData,binaryData_index);
	}
	diffuse_color=new float[4]; //创建用于存储散射光数据的数组
	for(int j=0; j<4; j++){ //循环获取散射光4 个色彩通道的值
		diffuse_color[j]= //读取并记录散射光的每个色彩通道值
				FileUtil::myReadFloat(binaryData,binaryData_index);
	}
	specular_color=new float[4]; //创建用于存储镜面光数据的数组
	for(int j=0; j<4; j++){ //循环获取镜面光4 个色彩通道的值
		specular_color[j]= //读取并记录镜面光的每个色彩通道值
				FileUtil::myReadFloat(binaryData,binaryData_index) ;
	}
	emissive_color=new float[4]; //创建用于存储自发光数据的数组
	for(int j=0; j<4; j++){ //循环获取自发光4 个色彩通道的值
		emissive_color[j]= //读取并记录自发光的每个色彩通道值
				FileUtil::myReadFloat(binaryData,binaryData_index);
	}
	shininess =FileUtil::myReadFloat(binaryData,binaryData_index); //读取粗糙度信息
	transparency =FileUtil::myReadFloat(binaryData,binaryData_index); //读取透明度信息
	FileUtil::myReadByte(binaryData,binaryData_index); //此数据在本案例中无用
	string tn=FileUtil::myReadString(binaryData,binaryData_index ,128); //读取材质对应纹理图的路径
	textureName =format(tn); //从文件路径中摘取出纹理图的文件名
	FileUtil::myReadString(binaryData,binaryData_index,128);//读取透明度贴图文件路径，本案例中无用
	TextureManager::texNames. //将纹理图名称存入纹理名称列表
			push_back("texture/"+texName2bntex(textureName));
}
MS3DMaterial::~MS3DMaterial()
{
	delete[] ambient_color;
	delete[] diffuse_color;
	delete[] specular_color;
	delete[] emissive_color;
}
string MS3DMaterial::format(string path)
{
	int offset = path.rfind("\\");
	int endset = path.rfind("g");
	if(offset!=string::npos&&endset!=string::npos)
	{
		return path.substr(offset+1,endset-1);
	}
	return path;
}
string MS3DMaterial::getName()
{
	return name;
}
