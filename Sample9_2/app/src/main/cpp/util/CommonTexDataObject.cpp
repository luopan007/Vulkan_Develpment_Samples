#include "CommonTexDataObject.h"
CommonTexDataObject::CommonTexDataObject(int width,int height,unsigned char* data,int dataByteCount)
{
    this->width=width;
    this->height=height;
    this->data=data;
    this->dataByteCount=dataByteCount;
}
CommonTexDataObject::~CommonTexDataObject()
{
    delete[] data;
}