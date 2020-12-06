#ifndef __FileUtil_H__
#define __FileUtil_H__
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include <string>
#include "TexDataObject.h"
#include "LandData.h"

using namespace std;
class FileUtil
{
public:
	static AAssetManager* aam;
	static void setAAssetManager(AAssetManager* aamIn);
	static string loadAssetStr(string fname);
	static TexDataObject* loadCommonTexData(string fname);
	static LandData* loadHdtData(string fname);//加载灰度图数据，生成地形mesh数据
};
#endif
