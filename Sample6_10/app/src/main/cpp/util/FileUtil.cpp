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
int fromBytesToInt(unsigned char* buff)
{
	int k=0;
	unsigned char* temp=(unsigned char*)(&k);
	temp[0]=buff[0];
	temp[1]=buff[1];
	temp[2]=buff[2];
	temp[3]=buff[3];
	return k;
}
TexArrayDataObject* FileUtil::load2DArrayTexData(string fname){ //加载 2D 纹理数组数据的方法
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);
	unsigned char* buf=new unsigned char[4]; //开辟长度为 4 字节的内存
	AAsset_read(asset, (void*)buf, 4); //读取纹理宽度数据字节
	int width=fromBytesToInt(buf); //转换为 int 型数值
	AAsset_read(asset, (void*)buf, 4); //读取纹理高度数据字节
	int height=fromBytesToInt(buf); //转换为 int 型数值
	AAsset_read(asset, (void*)buf, 4); //读取纹理数组长度数据字节
	int length=fromBytesToInt(buf); //转换为 int 型数值
	unsigned char* data=new unsigned char[width*height*length*4]; //开辟纹理数据存储内存
	AAsset_read(asset, (void*)data, width*height*length*4); //读取纹理数据
	TexArrayDataObject* ctdo=new TexArrayDataObject(width,height,length,data); //创建纹理数据对象
	return ctdo; //返回结果
}

