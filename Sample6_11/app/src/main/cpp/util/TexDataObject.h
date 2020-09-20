#ifndef VULKANEXBASE_COMMONTEXDATAOBJECT_H
#define VULKANEXBASE_COMMONTEXDATAOBJECT_H
class TexDataObject
{
  public:
    int width;
    int height;
    unsigned char* data;
    int dataByteCount;
    TexDataObject(int width,int height,unsigned char* data,int dataByteCount);
    ~TexDataObject();
};
#endif
