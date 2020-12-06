#ifndef __FileUtil_H__
#define __FileUtil_H__
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include <string>
#include "TexDataObject.h"
using namespace std;
typedef struct SpvDataStruct{
	int size;
	uint32_t* data;
} SpvData;
class FileUtil
{
  public:
	static AAssetManager* aam;
	static void setAAssetManager(AAssetManager* aamIn);
	static string loadAssetStr(string fname);
	static TexDataObject* loadCommonTexData(string fname);
	static SpvData& loadSPV(string fname);
};
#endif
