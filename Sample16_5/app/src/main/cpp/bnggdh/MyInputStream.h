#ifndef _MYINPUTSTREAM_H
#define _MYINPUTSTREAM_H
#include <string>
#include <android/asset_manager.h>
using namespace std;
class MyInputStream {
public:
    string sourceName;
    static AAssetManager* aam;
    static void setAAssetManager(AAssetManager* aamIn);
    unsigned char* binaryData;
    int binaryData_index=0;
    unsigned char* loadBinary();
    int myReadInt();
    int* myReadInts(int length);
    short myReadUnsignedShort();
    int myReadByte();
    float myReadFloat();
    float* myReadFloats(int length);
    string myReadString();
    MyInputStream(string sourceName);
    ~MyInputStream();
};
#endif
