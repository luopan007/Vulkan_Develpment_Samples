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
CommonTexDataObject* FileUtil::loadCommonTexData(string fname)
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);
	unsigned char* buf=new unsigned char[4];
	AAsset_read(asset, (void*)buf, 4);
	int width=fromBytesToInt(buf);
	AAsset_read(asset, (void*)buf, 4);
	int height=fromBytesToInt(buf);
	unsigned char* data=new unsigned char[width*height*4];
	AAsset_read(asset, (void*)data, width*height*4);
	CommonTexDataObject* ctdo=new CommonTexDataObject(width,height,data,width*height*4);
	return ctdo;
}
LandData* FileUtil::loadHdtData(string fname) { //加载灰度图数据的方法
	unsigned char *buf = new unsigned char[4]; //用于存放灰度图的宽度数据
	unsigned char *buf2 = new unsigned char[4]; //用于存放灰度图的高度数据
	AAsset *asset = AAssetManager_open(aam, fname.c_str(),
									   AASSET_MODE_UNKNOWN); //创建AAsset 对象
	AAsset_read(asset, (void *) buf, 4); //读取4 个字节存入宽度buf
	int width = fromBytesToInt(buf); //转换为int 型数值
	AAsset_read(asset, (void *) buf2, 4); //读取4 个字节存入高度buf
	int height = fromBytesToInt(buf2); //转换为int 型数值
	unsigned char *data = new unsigned char[width * height]; //开辟灰度图数据储存内存
	AAsset_read(asset, (void *) data, width * height); //读取灰度图数据
	LandData *land = new LandData(width, height, data); //创建灰度图数据对象
	return land; //返回对象指针
}
