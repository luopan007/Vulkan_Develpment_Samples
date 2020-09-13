#include "FileUtil.h"
#include <assert.h>

AAssetManager *FileUtil::aam;

void FileUtil::setAAssetManager(AAssetManager *aamIn) {
    aam = aamIn;
}

string FileUtil::loadAssetStr(string fname) {
    AAsset *asset = AAssetManager_open(aam, fname.c_str(), AASSET_MODE_UNKNOWN);
    off_t fileSize = AAsset_getLength(asset);
    unsigned char *data = (unsigned char *) malloc(fileSize + 1);
    data[fileSize] = '\0';
    int readBytesCount = AAsset_read(asset, (void *) data, fileSize);
    assert(readBytesCount == fileSize);
    std::string resultStr((const char *) data);
    return resultStr;
}

SpvData &FileUtil::loadSPV(string fname) { //加载Assets 文件夹下的SPIR-V 数据文件
    AAsset *asset = AAssetManager_open(aam, fname.c_str(), AASSET_MODE_STREAMING);
    assert(asset);
    size_t size = AAsset_getLength(asset); //获取SPIR-V 数据文件的总字节数
    assert(size > 0); //检查总字节数是否大于0
    SpvData spvData; //构建SpvData 结构体实例
    spvData.size = size; //设置SPIR-V 数据总字节数
    spvData.data = (uint32_t *) (malloc(size)); //分配相应字节数的内存
    AAsset_read(asset, spvData.data, size); //从文件中加载数据进入内存
    AAsset_close(asset); //关闭AAsset 对象
    return spvData; //返回SpvData 结构体实例
}

