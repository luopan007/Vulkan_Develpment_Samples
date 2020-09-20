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
TexDataObject* FileUtil::load_RGBA8_ETC2_EAC_TexData(string fname){//加载 ETC2 压缩纹理数据
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN); //创建 AAsset 对象
	int byteCount=AAsset_getLength(asset)-16; //纹理数据字节数
	unsigned char* buf=new unsigned char[8]; //开辟长度为 8 字节的内存
	AAsset_read(asset, (void*)buf, 8); //读取文件头前 8 个字节抛弃
	AAsset_read(asset, (void*)buf, 4); //再读取 4 个字节抛弃
	AAsset_read(asset, (void*)buf, 2); //读取纹理宽度数据字节
	int width=fromBytesToShort(buf); //转换为 int 型数值
	AAsset_read(asset, (void*)buf, 2); //读取纹理高度数据字节
	int height=fromBytesToShort(buf); //转换为 int 型数值
	unsigned char* data=new unsigned char[byteCount]; //开辟纹理数据存储内存
	AAsset_read(asset, (void*)data, byteCount); //读取纹理数据
	return new TexDataObject(width,height,data,byteCount); //返回结果
}

