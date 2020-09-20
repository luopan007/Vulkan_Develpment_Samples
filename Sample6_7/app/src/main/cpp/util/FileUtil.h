#ifndef __FileUtil_H__
#define __FileUtil_H__
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include <string>
#include "TexDataObject.h"
using namespace std;
class FileUtil
{
public:
	static AAssetManager* aam;
	static void setAAssetManager(AAssetManager* aamIn);
	static string loadAssetStr(string fname);
	static TexDataObject* load_RGBA8_ETC2_EAC_TexData(string fname);
};
#endif
