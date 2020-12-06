/*
 * MS3DHeader.cpp
 *
 *  Created on: 2015-9-4
 *      Author: Administrator
 */
#include "MS3DHeader.h"
#include "FileUtil.h"
#include <string>
#include <android/log.h>
using namespace std;
MS3DHeader::MS3DHeader(unsigned char* binaryData,int* binaryData_index)
{
	id = FileUtil::myReadString(binaryData,binaryData_index,10);
	version = FileUtil::myReadInt(binaryData,binaryData_index);
}
MS3DHeader::~MS3DHeader()
{
}
string MS3DHeader::getId()
{
	return id;
}
int MS3DHeader::getVersion()
{
	return version;
}
