#include "FileUtil.h"
#include <assert.h>

AAssetManager* FileUtil::aam;

void FileUtil::setAAssetManager(AAssetManager* aamIn)//初始化AAssetManager对象
{
	aam=aamIn;//给AAssetManager对象赋值
}

//加载Assets文件夹下的指定文本性质文件内容作为字符串返回
string FileUtil::loadAssetStr(string fname)
{
	//创建AAsset对象
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);
	//获取AAsset对象的长度(以字节计)
	off_t fileSize = AAsset_getLength(asset);
	//分配内存（+1是因为字符串最后需要\0）
	unsigned char* data = (unsigned char*) malloc(fileSize + 1);
	//设置最后一个字节为字符串结束符'\0'
	data[fileSize] = '\0';
	//读取文件内容
	int readBytesCount = AAsset_read(asset, (void*)data, fileSize);
	assert(readBytesCount==fileSize);
	//产生结果字符串
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

TexDataObject* FileUtil::loadCommonTexData(string fname){
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



SpvData& FileUtil::loadSPV(string fname)
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_STREAMING);//创建AAsset对象

	assert(asset);
	size_t size = AAsset_getLength(asset);
	assert(size > 0);

	SpvData spvData;
	spvData.size=size;
	spvData.data=(uint32_t*)(malloc(size));

	AAsset_read(asset, spvData.data, size);
	AAsset_close(asset);

	return spvData;
}

