#include "FlatData.h"
#include <vector>
#include <math.h>
#include <string.h>
float* FlatData::vdata;
int FlatData::dataByteCount;
int FlatData::vCount;
void  FlatData::genVertexData() {
    int cols = 12; //列数
    int rows = cols*3/4; //行数
    std::vector<float> alVertix; //存放顶点坐标数据的列表
    std::vector<float> alTexCoor; //存放纹理坐标数据的列表
    float width = 12; //旗帜矩形的宽度
    float UNIT_SIZE = width/cols; //小矩形的单位宽度
    for (int i = 0; i < cols; i++){ //遍历各个切分列
        for (int j = 0; j < rows; j++){ //遍历各个切分行
            float zsx = -UNIT_SIZE*cols / 2 + i*UNIT_SIZE; //当前小矩形左上侧点X 坐标
            float zsy = UNIT_SIZE*rows / 2 - j*UNIT_SIZE; //当前小矩形左上侧点Y 坐标
            float zsz = 0; //当前小矩形左上侧点Z 坐标
            alVertix.push_back(zsx);
            alVertix.push_back(zsy);
            alVertix.push_back(zsz);
            alVertix.push_back(zsx);
            alVertix.push_back(zsy - UNIT_SIZE);
            alVertix.push_back(zsz);
            alVertix.push_back(zsx + UNIT_SIZE);
            alVertix.push_back(zsy);
            alVertix.push_back(zsz);
            alVertix.push_back(zsx + UNIT_SIZE);
            alVertix.push_back(zsy);
            alVertix.push_back(zsz);
            alVertix.push_back(zsx);
            alVertix.push_back(zsy - UNIT_SIZE);
            alVertix.push_back(zsz);
            alVertix.push_back(zsx + UNIT_SIZE);
            alVertix.push_back(zsy - UNIT_SIZE);
            alVertix.push_back(zsz);
        }
    }
    float sizew = 1.0f / cols; //每个小矩形的纹理S 坐标跨度
    float sizeh = 1.0f / rows; //每个小矩形的纹理T 坐标跨度
    for (int i = 0; i<cols; i++){ //遍历各个切分列
        for (int j = 0; j<rows; j++){ //遍历各个切分行
            float s = i*sizew; //当前小矩形左上侧点纹理S 坐标
            float t = j*sizeh; //当前小矩形左上侧点纹理T 坐标
            alTexCoor.push_back(s);
            alTexCoor.push_back(t);
            alTexCoor.push_back(s);
            alTexCoor.push_back(t + sizeh);
            alTexCoor.push_back(s + sizew);
            alTexCoor.push_back(t);
            alTexCoor.push_back(s + sizew);
            alTexCoor.push_back(t);
            alTexCoor.push_back(s);
            alTexCoor.push_back(t + sizeh);
            alTexCoor.push_back(s + sizew);
            alTexCoor.push_back(t + sizeh);
        }
    }
    vCount = cols*rows * 6; //计算顶点数量
    dataByteCount = vCount * 5 * sizeof(float); //计算顶点数据总字节数
    vdata = new float[vCount*5]; //创建存放顶点数据的数组
    int index = 0; //辅助索引
    for (int i = 0; i < vCount; i++) {
        vdata[index++] = alVertix[i * 3 + 0];
        vdata[index++] = alVertix[i * 3 + 1];
        vdata[index++] = alVertix[i * 3 + 2];
        vdata[index++] = alTexCoor[i * 2 + 0];
        vdata[index++] = alTexCoor[i * 2 + 1];
    }
}
