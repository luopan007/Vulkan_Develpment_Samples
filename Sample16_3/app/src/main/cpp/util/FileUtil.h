#ifndef __FileUtil_H__
#define __FileUtil_H__
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include "TexDataObject.h"
#include <string>
using namespace std;
class FileUtil
{
  public:
	static AAssetManager* aam;
	static void setAAssetManager(AAssetManager* aamIn);
	static string loadAssetStr(string fname);
	static TexDataObject* loadCommonTexData(string fname);
	static unsigned char* loadBinary(std::string fname);
	static int myReadInt(unsigned char* binaryData, int* binaryData_index);
	static short myReadUnsignedShort(unsigned char* binaryData, int* binaryData_index);
	static int myReadByte(unsigned char* binaryData, int* binaryData_index);
	static float myReadFloat(unsigned char* binaryData, int* binaryData_index);
	static string myReadString(unsigned char* binaryData, int* binaryData_index,int length);
};
#endif
