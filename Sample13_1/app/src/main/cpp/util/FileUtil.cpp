#include "FileUtil.h"
#include <assert.h>
#include "../bndev/mylog.h"

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

TexDataObject* FileUtil::loadCubemapTexData(string fname)
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);
	unsigned char* buf=new unsigned char[4];

	AAsset_read(asset, (void*)buf, 4);
	int width=fromBytesToInt(buf);

	AAsset_read(asset, (void*)buf, 4);
	int height=fromBytesToInt(buf);

	AAsset_read(asset, (void*)buf, 4);
	int count=fromBytesToInt(buf);
	assert(count==6);

	unsigned char* data=new unsigned char[6*width*height*4];
	AAsset_read(asset, (void*)data, 6*width*height*4);

	TexDataObject* ctdo=new TexDataObject(width,height,data,6*width*height*4);

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
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);

	int byteCount=AAsset_getLength(asset)-16;
	unsigned char* buf=new unsigned char[8];

	AAsset_read(asset, (void*)buf, 8);

	AAsset_read(asset, (void*)buf, 4);

	AAsset_read(asset, (void*)buf, 2);
	int width=fromBytesToShort(buf);

	AAsset_read(asset, (void*)buf, 2);
	int height=fromBytesToShort(buf);

	unsigned char* data=new unsigned char[byteCount];
	AAsset_read(asset, (void*)data, byteCount);

	return new TexDataObject(width,height,data,byteCount);
}

