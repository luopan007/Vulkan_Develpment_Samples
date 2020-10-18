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

int fromBytesToShort(unsigned char* buff)
{
	int k=0;
	unsigned char* temp=(unsigned char*)(&k);
	temp[0]=buff[1];
	temp[1]=buff[0];
	temp[2]=0;
	temp[3]=0;
	return k;
}

//加载非压缩RGBA四通道普通纹理数据
TexDataObject* FileUtil::loadCommonTexData(string fname)
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);//创建AAsset对象

	unsigned char* buf=new unsigned char[4];
    //读取纹理宽度
	AAsset_read(asset, (void*)buf, 4);
	int width=fromBytesToInt(buf);
	//读取纹理高度
	AAsset_read(asset, (void*)buf, 4);
	int height=fromBytesToInt(buf);
	//读取纹理数据
	unsigned char* data=new unsigned char[width*height*4];
	AAsset_read(asset, (void*)data, width*height*4);
	//创建纹理数据对象
	TexDataObject* ctdo=new TexDataObject(width,height,data,width*height*4,COMMON);
	//返回结果
	return ctdo;
}

/*
4 byte magic number: "PKM "
2 byte version "10"
2 byte data type: 0 (ETC1_RGB_NO_MIPMAPS)
16 bit big endian extended width
16 bit big endian extended height
16 bit big endian original width
16 bit big endian original height
*/
TexDataObject* FileUtil::load_RGBA8_ETC2_EAC_TexData(string fname)
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);//创建AAsset对象
	//获取纹理数据字节数
	int byteCount=AAsset_getLength(asset)-16;
	unsigned char* buf=new unsigned char[8];
	//读取文件头前8个字节抛弃
	AAsset_read(asset, (void*)buf, 8);
	//再读取4个字节抛弃
	AAsset_read(asset, (void*)buf, 4);
	//读取两个字节的纹理宽度
	AAsset_read(asset, (void*)buf, 2);
	int width=fromBytesToShort(buf);
	//读取两个字节的纹理高度
	AAsset_read(asset, (void*)buf, 2);
	int height=fromBytesToShort(buf);
	//读取纹理数据
	unsigned char* data=new unsigned char[byteCount];
	AAsset_read(asset, (void*)data, byteCount);
	//返回结果
	return new TexDataObject(width,height,data,byteCount,RGBA8_ETC2_EAC);
}

