#include "CommonTexDataObject.h"

CommonTexDataObject::CommonTexDataObject(int width,int height,unsigned char* data)
{
    this->width=width;
    this->height=height;
    this->data=data;
}

CommonTexDataObject::~CommonTexDataObject()
{
    delete data;
}