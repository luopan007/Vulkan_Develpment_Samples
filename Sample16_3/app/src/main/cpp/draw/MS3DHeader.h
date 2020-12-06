/*
 * MS3DHeader.h
 *
 *  Created on: 2015-9-4
 *      Author: Administrator
 */
#ifndef MS3DHEADER_H_
#define MS3DHEADER_H_
#include <string>
using namespace std;
class MS3DHeader{
private:
	string id;
	int version;
public:
	MS3DHeader(unsigned char* binaryData,int* binaryData_index);
	~MS3DHeader();
	string getId();
	int getVersion();
};
#endif /* MS3DHEADER_H_ */
