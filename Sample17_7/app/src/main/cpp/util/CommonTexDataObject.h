#ifndef VULKANEXBASE_COMMONTEXDATAOBJECT_H
#define VULKANEXBASE_COMMONTEXDATAOBJECT_H


class CommonTexDataObject
{
  public:
    int width;
    int height;
    unsigned char* data;

    CommonTexDataObject(int width,int height,unsigned char* data);
    ~CommonTexDataObject();
};

#endif


