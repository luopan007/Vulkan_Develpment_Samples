#include "MyInputStream.h"
AAssetManager* MyInputStream::aam;
void MyInputStream::setAAssetManager(AAssetManager* aamIn) {
    aam=aamIn;
}
MyInputStream::MyInputStream(string sourceName) {
    this->sourceName=sourceName;
    binaryData=loadBinary();
}
MyInputStream::~MyInputStream() {
    delete[] binaryData;
}
unsigned char* MyInputStream::loadBinary(){
    AAsset* asset =AAssetManager_open(aam,sourceName.c_str(),AASSET_MODE_UNKNOWN);
    off_t fileSize = AAsset_getLength(asset);
    unsigned char* data = (unsigned char*) malloc(fileSize + 1);
    data[fileSize] = '\0';
    int size = AAsset_read(asset, (void*)data, fileSize);
    return data;
}
int MyInputStream::myReadInt(){
    int readInt = 0;
    unsigned char * readInt_char = (unsigned char*)&readInt;
    for(int i = 0; i < sizeof(int); i++){
        readInt_char[i] = (unsigned char)binaryData[binaryData_index];
        binaryData_index++;
    }
    return readInt;
}
int* MyInputStream::myReadInts(int length){
    int *result=new int[length]();
    for(int i = 0; i < length; i++){
        result[i]=myReadInt();
    }
    return result;
}
short MyInputStream::myReadUnsignedShort(){
    short readShort = 0;
    unsigned char * readShort_char = (unsigned char*)&readShort;
    for(int i = 0; i < sizeof(short); i++){
        readShort_char[i] = (unsigned char)binaryData[binaryData_index];
        binaryData_index++;
    }
    return readShort;
}
int MyInputStream::myReadByte(){
    int readByte=0;
    unsigned char * readByte_char = (unsigned char*)&readByte;
    for(int i = 0; i < 1; i++){
        readByte_char[i] = (unsigned char)binaryData[binaryData_index];
        binaryData_index++;
    }
    return readByte;
}
float MyInputStream::myReadFloat(){
    float readFloat = 0.0f;
    unsigned char * readFloat_char = (unsigned char*)&readFloat;
    for(int i = 0; i < sizeof(float); i++){
        readFloat_char[i] = (unsigned char)binaryData[binaryData_index];
        binaryData_index++;
    }
    return readFloat;
}
float* MyInputStream::myReadFloats(int length){
    float* result = new float[length]();
    for(int i = 0; i < length; i++){
        result[i]=myReadFloat();
    }
    return result;
}
string MyInputStream::myReadString(){
    int len = myReadInt();
    if(len == 0){
        string s;
        return s;
    }
    unsigned char* ansChar = new unsigned char[len+1];
    for(int i = 0; i < len; i++){
        ansChar[i] = (unsigned char)binaryData[binaryData_index];
        binaryData_index++;
    }
    ansChar[len] = '\0';
    string str((const char*)ansChar);
    return str;
}
