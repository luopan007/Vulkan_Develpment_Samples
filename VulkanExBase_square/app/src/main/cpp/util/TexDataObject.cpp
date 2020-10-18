#include "TexDataObject.h"

TexDataObject::TexDataObject(int width,int height,unsigned char* data,int dataByteCount,BNTexType type)
{
    this->width=width;
    this->height=height;
    this->data=data;
    this->type=type;
    this->dataByteCount=dataByteCount;
}

TexDataObject::~TexDataObject()
{
    delete data;
}