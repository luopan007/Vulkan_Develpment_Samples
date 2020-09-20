#ifndef VULKANEXBASE_ThreeDTexDataObject_H
#define VULKANEXBASE_ThreeDTexDataObject_H //防止被重复引用
class ThreeDTexDataObject{
public:
    int width; //纹理宽度
    int height; //纹理高度
    int depth; //纹理深度
    unsigned char* data; //纹理的数据总字节数
    int dataByteCount; //指向纹理数据存储内存首地址的指针
    ThreeDTexDataObject(int width,int height,int depth,unsigned char* data); //构造函数
    ~ThreeDTexDataObject(); //析构函数
};
#endif
