#include "TexArrayDataObject.h" //引入头文件
TexArrayDataObject::TexArrayDataObject(int width,int height,int length,unsigned char* data){
    this->width=width; //纹理的宽度
    this->height=height; //纹理的高度
    this->length=length; //纹理数组的长度
    this->data=data; //纹理数据内存首地址指针
    this->dataByteCount = width*height*length * 4; //纹理数据总字节数
}
TexArrayDataObject::~TexArrayDataObject(){ //析构函数
    delete[] data; //释放纹理数据内存
}
