#include "LandData.h"
#define LAND_SPAN 30
#define LAND_HIGHEST 300
#define LAND_ADJUST_Y 0
LandData::LandData(int width, int height, unsigned char* data) { //构造函数
    float **gdz = new float*[width]; //创建实际高度值二维数组第一层
    for (int i = 0; i<width; ++i) gdz[i] = new float[height]; //创建实际高度值二维数组第二层
    for (int i = 0; i<width; i++) { //遍历所有列
        for (int j = 0; j<height; j++) { //遍历所有行
            int h = data[i*height + j]; //获取当前行列的高度参数值
            gdz[i][j] = h*LAND_HIGHEST / 255.0f + LAND_ADJUST_Y;}}//折算出实际高度值
    int rows = height - 1; //地形网格行数
    int cols = width - 1; //地形网格列数
    int gzCount = rows*cols; //地形网格格子数
    vCount = gzCount * 2 * 3; //地形网格顶点数
    vData = new float[vCount * 5]; //顶点数据数组
    float sSpan = 32.0f / cols; //纹理S 坐标单元跨度
    float tSpan = 32.0f / rows; //纹理T 坐标单元跨度
    float xStart = -LAND_SPAN*cols / 2.0f; //地形网格X 坐标起始值
    float zStart = -LAND_SPAN*rows / 2.0f; //地形网格Z 坐标起始值
    int indexTemp = 0; //辅助索引
    for (int i = 0; i<cols; i++) { //遍历地形网格所有列
        for (int j = 0; j<rows; j++) { //遍历地形网格所有行
            float x0 = xStart + LAND_SPAN*i; //当前网格0 号顶点X 坐标
            float y0 = gdz[i][j]; //当前网格0 号顶点Y 坐标
            float z0 = zStart + LAND_SPAN*j; //当前网格0 号顶点Z 坐标
            float s0 = sSpan*i; float t0 = tSpan*j; //当前网格0 号顶点纹理S、T 坐标
            float x1 = xStart + LAND_SPAN*(i + 1);
            float y1 = gdz[i + 1][j];
            float z1 = zStart + LAND_SPAN*j;
            float s1 = sSpan*(i + 1);
            float t1 = tSpan*j;
            float x2 = xStart + LAND_SPAN*i;
            float y2 = gdz[i][j + 1];
            float z2 = zStart + LAND_SPAN*(j + 1);
            float s2 = sSpan*i;
            float t2 = tSpan*(j + 1);
            float x3 = xStart + LAND_SPAN*(i + 1);
            float y3 = gdz[i + 1][j + 1];
            float z3 = zStart + LAND_SPAN*(j + 1);
            float s3 = sSpan*(i + 1);
            float t3 = tSpan*(j + 1);
            vData[indexTemp++] = x0; //将0 号顶点X 坐标存入结果数组
            vData[indexTemp++] = y0; //将0 号顶点Y 坐标存入结果数组
            vData[indexTemp++] = z0; //将0 号顶点Z 坐标存入结果数组
            vData[indexTemp++] = s0; vData[indexTemp++] = t0;//将0 号顶点S、T 纹理坐标存入数组
            vData[indexTemp++] = x3;
            vData[indexTemp++] = y3;
            vData[indexTemp++] = z3;
            vData[indexTemp++] = s3;
            vData[indexTemp++] = t3;
            vData[indexTemp++] = x1;
            vData[indexTemp++] = y1;
            vData[indexTemp++] = z1;
            vData[indexTemp++] = s1;
            vData[indexTemp++] = t1;
            vData[indexTemp++] = x0;
            vData[indexTemp++] = y0;
            vData[indexTemp++] = z0;
            vData[indexTemp++] = s0;
            vData[indexTemp++] = t0;
            vData[indexTemp++] = x2;
            vData[indexTemp++] = y2;
            vData[indexTemp++] = z2;
            vData[indexTemp++] = s2;
            vData[indexTemp++] = t2;
            vData[indexTemp++] = x3;
            vData[indexTemp++] = y3;
            vData[indexTemp++] = z3;
            vData[indexTemp++] = s3;
            vData[indexTemp++] = t3;
        }
    }
    for(int i = 0; i < height; i ++)
        delete [] gdz[i];
    delete [] gdz;
}
