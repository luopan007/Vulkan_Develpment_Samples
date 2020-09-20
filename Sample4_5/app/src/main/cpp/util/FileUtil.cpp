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



