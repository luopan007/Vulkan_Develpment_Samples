#include "TexDataObject.h" //引入头文件
TexDataObject::TexDataObject(int width,int height,unsigned char* data,int dataByteCount){
    this->width=width; //纹理的宽度
    this->height=height; //纹理的高度
    this->data=data; //纹理数据首地址指针
    this->dataByteCount=dataByteCount; //纹理数据总字节数
}
TexDataObject::~TexDataObject(){ //析构函数
    delete[] data; //释放纹理数据内存
}
