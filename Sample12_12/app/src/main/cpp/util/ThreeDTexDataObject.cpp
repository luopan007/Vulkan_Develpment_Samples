#include "ThreeDTexDataObject.h" //引入头文件
ThreeDTexDataObject::ThreeDTexDataObject(int width,int height,int depth,unsigned char* data){
    this->width=width; //记录纹理的宽度
    this->height=height; //记录纹理的高度
    this->depth=depth; //记录纹理的深度
    this->data=data; //记录纹理数据内存首地址指针
    this->dataByteCount = width*height*depth * 4; //记录纹理数据总字节数
}
ThreeDTexDataObject::~ThreeDTexDataObject(){ //析构函数
    delete[] data; //释放纹理数据内存
}
