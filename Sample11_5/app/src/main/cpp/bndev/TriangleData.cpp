#include "TriangleData.h"
#include <vector>
#include <math.h>
#include <string.h>
#define  PI 3.1415926
float* TriangleData::vdata;//数据数组首地址指针
int TriangleData::dataByteCount;//数据所占总字节数量
int TriangleData::vCount;//顶点数量
void  TriangleData::genVertexData() {//顶点数据生成方法
    std::vector<float> alVertex;//存放顶点数据的vector
    std::vector<float> alTexoor;//存放纹理坐标的vector
    float edgeLength=24;//三角形边长
    int levelNum=40;//三角形组层数
    float perLength = edgeLength/levelNum;//小三角形的边长
    for(int i=0;i<levelNum;i++)//循环每一层生成小三角形
    {
        //当前层顶端边数
        int currTopEdgeNum=i;
        //当前层底端边数
        int currBottomEdgeNum=i+1;
        //每个三角形的高度
        float currTrangleHeight=(float) (perLength*sin(PI/3));
        //当前层顶端最左边点的坐标
        float topEdgeFirstPointX=-perLength*currTopEdgeNum/2;
        float topEdgeFirstPointY=-i*currTrangleHeight;
        float topEdgeFirstPointZ=0;

        //当前层底端最左边点的坐标
        float bottomEdgeFirstPointX=-perLength*currBottomEdgeNum/2;
        float bottomEdgeFirstPointY=-(i+1)*currTrangleHeight;
        float bottomEdgeFirstPointZ=0;
        //---------------纹理----------------
        float horSpan=1/(float)levelNum;//横向纹理的偏移量
        float verSpan=1/(float)levelNum;//纵向纹理的偏移量
        //当前层顶端最左边点的纹理ST坐标
        float topFirstS=0.5f-currTopEdgeNum*horSpan/2;
        float topFirstT=i*verSpan;
        ////当前层底端最左边点的纹理ST坐标
        float bottomFirstS=0.5f-currBottomEdgeNum*horSpan/2;
        float bottomFirstT=(i+1)*verSpan;
        //循环产生当前层各个上三角形的顶点数据
        for(int j=0;j<currBottomEdgeNum;j++)
        {
            //当前三角形顶端点的X、Y、Z坐标
            float topX=topEdgeFirstPointX+j*perLength;
            float topY=topEdgeFirstPointY;
            float topZ=topEdgeFirstPointZ;
            //当前三角形顶端点的S、T纹理坐标
            float topS=topFirstS+j*horSpan;
            float topT=topFirstT;

            //当前三角形左下侧点的X、Y、Z坐标
            float leftBottomX=bottomEdgeFirstPointX+j*perLength;
            float leftBottomY=bottomEdgeFirstPointY;
            float leftBottomZ=bottomEdgeFirstPointZ;
            //当前三角形左下侧点的S、T纹理坐标
            float leftBottomS=bottomFirstS+j*horSpan;
            float leftBottomT=bottomFirstT;

            //当前三角形右下侧点的X、Y、Z坐标
            float rightBottomX=leftBottomX+perLength;
            float rightBottomY=bottomEdgeFirstPointY;
            float rightBottomZ=bottomEdgeFirstPointZ;
            //当前三角形右下侧点的S、T纹理坐标
            float rightBottomS=leftBottomS+horSpan;
            float rightBottomT=leftBottomT;
            //将当前三角形顶点数据按照逆时针顺序送入顶点坐标、纹理坐标列表
            //---顶点绘制方式
            alVertex.push_back(topX);alVertex.push_back(topY);alVertex.push_back(topZ);
            alVertex.push_back(leftBottomX);alVertex.push_back(leftBottomY);alVertex.push_back(leftBottomZ);
            alVertex.push_back(rightBottomX);alVertex.push_back(rightBottomY);alVertex.push_back(rightBottomZ);
            //-------纹理绘制方式
            alTexoor.push_back(topS);alTexoor.push_back(topT);
            alTexoor.push_back(leftBottomS);alTexoor.push_back(leftBottomT);
            alTexoor.push_back(rightBottomS);alTexoor.push_back(rightBottomT);

        }
        for(int k=0;k<currTopEdgeNum;k++)
        {//循环产生当前层各个下三角形的顶点数据
            float leftTopX=topEdgeFirstPointX+k*perLength; 	//当前三角形左上侧
            float leftTopY=topEdgeFirstPointY;				//点的X、Y、Z坐标
            float leftTopZ=topEdgeFirstPointZ;
            float leftTopS=topFirstS+k*horSpan;				//当前三角形左上侧点的S、T纹理坐标
            float leftTopT=topFirstT;

            float bottomX=bottomEdgeFirstPointX+(k+1)*perLength;//当前三角形底端点的X、Y、Z坐标
            float bottomY=bottomEdgeFirstPointY;
            float bottomZ=bottomEdgeFirstPointZ;
            float bottomS=bottomFirstS+(k+1)*horSpan;		//当前三角形右底端点的S、T纹理坐标
            float bottomT=bottomFirstT;

            float rightTopX=leftTopX+perLength; 			//当前三角形右上侧点的X、Y、Z坐标
            float rightTopY=leftTopY;
            float rightTopZ=leftTopZ;
            float rightTopS=leftTopS+horSpan;				//当前三角形右上侧点的S、T纹理坐标
            float rightTopT=topFirstT;
            //逆时针卷绕-----
            alVertex.push_back(leftTopX);alVertex.push_back(leftTopY);alVertex.push_back(leftTopZ);
            alVertex.push_back(bottomX);alVertex.push_back(bottomY);alVertex.push_back(bottomZ);
            alVertex.push_back(rightTopX);alVertex.push_back(rightTopY);alVertex.push_back(rightTopZ);

            alTexoor.push_back(leftTopS);alTexoor.push_back(leftTopT);
            alTexoor.push_back(bottomS);alTexoor.push_back(bottomT);
            alTexoor.push_back(rightTopS);alTexoor.push_back(rightTopT);
        }
    }
    vCount = alVertex.size()/3;//顶点数量
    dataByteCount = vCount * 5 * sizeof(float);//数据所占内存总字节数
    vdata=new float[vCount*5];
    int index=0;//辅助索引
    for(int i=0;i<vCount;i++){
        vdata[index++]=alVertex[i*3+0];
        vdata[index++]=alVertex[i*3+1];
        vdata[index++]=alVertex[i*3+2];

        vdata[index++]=alTexoor[i*2+0];
        vdata[index++]=alTexoor[i*2+1];
    }
}
