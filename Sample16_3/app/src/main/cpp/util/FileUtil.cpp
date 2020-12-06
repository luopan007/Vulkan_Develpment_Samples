#include "FileUtil.h"
#include <assert.h>
#include <jni.h>
#include <dlfcn.h>
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
unsigned char* FileUtil::loadBinary(std::string fname){
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);
	off_t fileSize = AAsset_getLength(asset);
	unsigned char* data = (unsigned char*) malloc(fileSize + 1);
	data[fileSize] = '\0';
	int size = AAsset_read(asset, (void*)data, fileSize);
	return data;
}
int FileUtil::myReadInt(unsigned char* binaryData, int* binaryData_index) {
	int readInt = 0;
	unsigned char * readInt_char = (unsigned char*)&readInt;
	for (int i = 0; i < sizeof(int); i++) {
		readInt_char[i] = (unsigned char)binaryData[*binaryData_index];
		(*binaryData_index)++;
	}
	return readInt;
}
short FileUtil::myReadUnsignedShort(unsigned char* binaryData, int* binaryData_index) {
	short readShort = 0;
	unsigned char * readShort_char = (unsigned char*)&readShort;
	for (int i = 0; i < sizeof(short); i++) {
		readShort_char[i] = (unsigned char)binaryData[*binaryData_index];
		(*binaryData_index)++;
	}
	return readShort;
}
int FileUtil::myReadByte(unsigned char* binaryData, int* binaryData_index)
{
	int readByte = 0;
	unsigned char * readByte_char = (unsigned char*)&readByte;
	for (int i = 0; i < 1; i++) {
		readByte_char[i] = (unsigned char)binaryData[*binaryData_index];
		(*binaryData_index)++;
	}
	return readByte;
}
float FileUtil::myReadFloat(unsigned char* binaryData, int* binaryData_index) {
	float readFloat = 0.0f;
	unsigned char * readFloat_char = (unsigned char*)&readFloat;
	for (int i = 0; i < sizeof(float); i++) {
		readFloat_char[i] = (unsigned char)binaryData[*binaryData_index];
		(*binaryData_index)++;
	}
	return readFloat;
}
string FileUtil::myReadString(unsigned char* binaryData, int* binaryData_index, int length) {
	int len = length;
	if (len == 0) {
		string s;
		return s;
	}
	unsigned char* ansChar = new unsigned char[len + 1];
	for (int i = 0; i < len; i++) {
		ansChar[i] = (unsigned char)binaryData[*binaryData_index];
		(*binaryData_index)++;
	}
	ansChar[len] = '\0';
	string str((const char*)ansChar);
	return str;
}
