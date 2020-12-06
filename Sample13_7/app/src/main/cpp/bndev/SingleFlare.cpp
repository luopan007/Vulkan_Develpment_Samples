#include "SingleFlare.h"
#include <vector>
using namespace std;
SingleFlare::SingleFlare(int idIn, float size, float distance, vector<float> color){
    this->idIn = idIn; //初始化光晕元素纹理序号
    this->distance = distance; //初始化光晕元素的距离
    this->size = size; //初始化光晕元素的原始尺寸值
    this->bSize = size; //初始化光晕元素的变换后的尺寸值
    this->color = color; //初始化光晕元素的颜色列表
    this->px = 0; //初始化光晕元素的x 坐标
    this->py = 0; //初始化光晕元素的y 坐标
}
