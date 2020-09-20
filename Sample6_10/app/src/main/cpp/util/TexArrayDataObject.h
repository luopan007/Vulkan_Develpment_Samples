#ifndef VULKANEXBASE_COMMONTEXDATAOBJECT_H
#define VULKANEXBASE_COMMONTEXDATAOBJECT_H //防止被重复引用
class TexArrayDataObject{
public:
    int width; //纹理宽度
    int height; //纹理高度
    int length; //纹理数组长度
    unsigned char* data; //纹理的数据总字节数
    int dataByteCount; //指向纹理数据存储内存首地址的指针
    TexArrayDataObject(int width,int height,int length,unsigned char* data); //构造函数
    ~TexArrayDataObject(); //析构函数
};
#endif
