

#ifndef VULKANEXBASE_ThreeDTexDataObject_H
#define VULKANEXBASE_ThreeDTexDataObject_H 
class ThreeDTexDataObject {
public:
    int width; 
    int height; 
    int depth; 
    unsigned char* data; 
    int dataByteCount; 
    ThreeDTexDataObject(int width, int height, int depth, unsigned char* data); 
    ~ThreeDTexDataObject(); 
};
#endif
