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
	static void storeData(string path,const char *buf, int num,int width,int height);
};
#endif
