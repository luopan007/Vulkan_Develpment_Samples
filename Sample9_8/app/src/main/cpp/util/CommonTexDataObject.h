#ifndef VULKANEXBASE_TEXDATAOBJECT_H
#define VULKANEXBASE_TEXDATAOBJECT_H
class CommonTexDataObject
{
public:
    int width;
    int height;
    int dataByteCount;
    unsigned char* data;
    CommonTexDataObject(int width,int height,unsigned char* data,int dataByteCount);
    ~CommonTexDataObject();
};
#endif
