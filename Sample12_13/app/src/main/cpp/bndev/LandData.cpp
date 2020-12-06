#include <mylog.h>
#include "LandData.h"

#define LAND_SPAN 30
#define LAND_HIGHEST 300
#define LAND_ADJUST_Y 0
LandData::LandData(int width, int height, unsigned char* data)
{
    //计算出所需的高度值数组
    float **gdz = new float*[width];
    for (int i = 0; i<width; ++i) gdz[i] = new float[height];
    for (int i = 0; i<width; i++)
    {
        for (int j = 0; j<height; j++)
        {
            int h = data[i*height + j];
            gdz[i][j] = h*LAND_HIGHEST / 255.0f + LAND_ADJUST_Y;
        }
    }
    //地形行数
    int rows = height - 1;
    //地形列数
    int cols = width - 1;
    //计算地形mesh格子数
    int gzCount = rows*cols;
    //mesh顶点数
    vCount = gzCount * 2 * 3;
    //顶点数据
    vData = new float[vCount * 5];
    //每格的纹理跨度
    float sSpan = 8.0f / cols;
    float tSpan = 8.0f / rows;
    LOGE("cols%f,rows%f",cols,rows);
    LOGE("sSpan%f,tSpan%f",sSpan,tSpan);
    //地形网格X起始值
    float xStart = -LAND_SPAN*cols / 2.0f;
    //地形网格Z起始值
    float zStart = -LAND_SPAN*rows / 2.0f;
    //
    //对行列地形网格进行遍历处理
    int indexTemp = 0;
    for (int i = 0; i<cols; i++)
    {
        for (int j = 0; j<rows; j++)
        {
            //0---1
            //| \ |
            //2---3
            //0号点的坐标及纹理坐标
            float x0 = xStart + LAND_SPAN*i;
            float y0 = gdz[i][j];
            float z0 = zStart + LAND_SPAN*j;
            float s0 = sSpan*i;
            float t0 = tSpan*j;

            //1号点的坐标及纹理坐标
            float x1 = xStart + LAND_SPAN*(i + 1);
            float y1 = gdz[i + 1][j];
            float z1 = zStart + LAND_SPAN*j;
            float s1 = sSpan*(i + 1);
            float t1 = tSpan*j;

            //2号点的坐标及纹理坐标
            float x2 = xStart + LAND_SPAN*i;
            float y2 = gdz[i][j + 1];
            float z2 = zStart + LAND_SPAN*(j + 1);
            float s2 = sSpan*i;
            float t2 = tSpan*(j + 1);

            //3号点的坐标及纹理坐标
            float x3 = xStart + LAND_SPAN*(i + 1);
            float y3 = gdz[i + 1][j + 1];
            float z3 = zStart + LAND_SPAN*(j + 1);
            float s3 = sSpan*(i + 1);
            float t3 = tSpan*(j + 1);

            //卷绕0-3-1三角形
            vData[indexTemp++] = x0;
            vData[indexTemp++] = y0;
            vData[indexTemp++] = z0;
            vData[indexTemp++] = s0;
            vData[indexTemp++] = t0;

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

            //卷绕0-2-3三角形
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
}
