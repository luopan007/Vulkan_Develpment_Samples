#include "FileUtil.h"
#include <assert.h>
AAssetManager* FileUtil::aam;
void FileUtil::setAAssetManager(AAssetManager* aamIn)
{
	aam=aamIn;
}
string FileUtil::loadAssetStr(string fname)
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);
	off_t fileSize = AAsset_getLength(asset);
	unsigned char* data = (unsigned char*) malloc(fileSize + 1);
	data[fileSize] = '\0';
	int readBytesCount = AAsset_read(asset, (void*)data, fileSize);
	assert(readBytesCount==fileSize);
	std::string resultStr((const char*)data);
	return resultStr;
}
int fromBytesToInt(unsigned char* buff){			//将字节序列转换为int值的方法
	int k=0;										//结果变量
	unsigned char* temp=(unsigned char*)(&k);		//将结果变量所占内存以字节序列模式访问
	temp[0]=buff[0];								//设置第1个字节的数据
	temp[1]=buff[1];								//设置第2个字节的数据
	temp[2]=buff[2]; 								//设置第3个字节的数据
	temp[3]=buff[3]; 								//设置第4个字节的数据
	return k;										//返回结果值
}

TexDataObject* FileUtil::loadCommonTexData(string fname){			//加载bntex纹理数据
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),
									  AASSET_MODE_UNKNOWN);			//创建AAsset对象
	unsigned char* buf=new unsigned char[4];						//开辟长度为4字节的内存
	AAsset_read(asset, (void*)buf, 4);								//读取纹理宽度数据字节
	int width=fromBytesToInt(buf);									//转换为int型数值
	AAsset_read(asset, (void*)buf, 4);								//读取纹理高度数据字节
	int height=fromBytesToInt(buf);									//转换为int型数值
	unsigned char* data=new unsigned char[width*height*4];			//开辟纹理数据存储内存
	AAsset_read(asset, (void*)data, width*height*4);				//读取纹理数据
	TexDataObject* ctdo=new TexDataObject(width,height,data,width*height*4);//创建纹理数据对象
	return ctdo;													//返回结果
}

