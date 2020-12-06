#ifndef VULKANEXBASE_TexDataObject_H
#define VULKANEXBASE_TexDataObject_H
typedef enum TexTypeENUM
{
    COMMON=0,           
    RGBA8_ETC2_EAC=1,   
} BNTexType;
class TexDataObject
{
public:
    int width;
    int height;
    int dataByteCount;
    unsigned char* data;
    BNTexType type;
    TexDataObject(int width,int height,unsigned char* data,int dataByteCount,BNTexType type);
    ~TexDataObject();
};
#endif
