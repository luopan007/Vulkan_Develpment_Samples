#ifndef __FileUtil_H__
#define __FileUtil_H__

#include <cstdint>
#include <android/asset_manager.h>
#include <string>

using namespace std;
typedef struct SpvDataStruct { //存储SPIR-V 数据的结构体
    int size; //SPIR-V 数据总字节数
    uint32_t *data; //指向SPIR-V 数据内存块首地址的指针
} SpvData;

class FileUtil {
public:
    static AAssetManager *aam;

    static void setAAssetManager(AAssetManager *aamIn);

    static string loadAssetStr(string fname);

    static SpvData &loadSPV(string fname);//加载Assets 文件夹下的SPIR-V 数据
};

#endif
