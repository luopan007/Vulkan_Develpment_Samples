#ifndef VULKANEXBASE_TexDataObject_H
#define VULKANEXBASE_TexDataObject_H

class TexDataObject
{
public:
    int width;
    int height;
    int dataByteCount;
    unsigned char* data;

    TexDataObject(int width, int height, unsigned char* data, int dataByteCount);
    ~TexDataObject();
};

#endif


