#include "FileUtil.h"
#include <assert.h>
#include <mylog.h>

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
TexDataObject* FileUtil::loadCommonTexData(string fname)
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);
	unsigned char* buf=new unsigned char[4];
	AAsset_read(asset, (void*)buf, 4);
	int width=fromBytesToInt(buf);
	AAsset_read(asset, (void*)buf, 4);
	int height=fromBytesToInt(buf);
	unsigned char* data=new unsigned char[width*height*4];
	AAsset_read(asset, (void*)data, width*height*4);
	TexDataObject* ctdo=new TexDataObject(width,height,data,width*height*4);
	return ctdo;
}
LandData* FileUtil::loadHdtData(string fname)//加载灰度图数据的方法
{
	unsigned char* buf = new unsigned char[4];//存放灰度图片宽度
	unsigned char* buf2 = new unsigned char[4];//存放灰度图片高度
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);//打开文件内容
	AAsset_read(asset, (void*)buf, 4);
	int width = fromBytesToInt(buf);//将buf转成一个int宽度
	AAsset_read(asset, (void*)buf2, 4);
	int height = fromBytesToInt(buf2);//将buf转成一个int宽度
	LOGE(" HDt %d %d\n", width, height);
	unsigned char* data = new unsigned char[width*height];
	AAsset_read(asset, (void*)data, width*height);
	LandData* ld = new LandData(width, height, data);
	return ld;
}
