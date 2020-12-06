#include "TreeTrunkData.h"
#include <vector>
#include "HelpFunction.h"
#include "mylog.h"
#include <math.h>
#include <string.h>
float* TreeTrunkData::vdata;
int TreeTrunkData::dataByteCount;
int TreeTrunkData::vCount;
float longitude_span=12;//经度切分的角度
/* @param bottom_radius	第一个节点底端半径
	@param joint_Height	第一个节点的顶端半径
	@param jointNum		每个节点的高度
	@param availableNum	节点的数量*/
void  TreeTrunkData::genVertexData(float bottom_radius,float joint_Height,int jointNum,int availableNum){
    std::vector<float> alVertix;//存放顶点坐标的vector
    std::vector<float> alTexCoor;//存放顶点坐标的vector
    float* texcoor;//存放纹理数据的数组
    for(int num=0;num<availableNum;num++){//循环计算出每节树干中的各个顶点
        //此节树干底端半径
        float temp_bottom_radius=bottom_radius*(jointNum-num)/(float)jointNum;
        //此节树干顶端半径
        float temp_top_radius=bottom_radius*(jointNum-(num+1))/(float)jointNum;
        //此节树干底端的y坐标
        float temp_bottom_height=num*joint_Height;
        //此节树干顶端的y坐标
        float temp_top_height=(num+1)*joint_Height;
        //循环一周，生成组成此节树干各个四边形的顶点坐标，并卷绕成三角形
        for(float hAngle=0;hAngle<360;hAngle=hAngle+longitude_span)
        {
            //当前四边形左上点X、Y、Z坐标
            float x0=(float) (temp_top_radius*cos(toRadians(hAngle)));
            float y0=temp_top_height;
            float z0=(float) (temp_top_radius*sin(toRadians(hAngle)));
            //当前四边形左下点X、Y、Z坐标
            float x1=(float) (temp_bottom_radius*cos(toRadians(hAngle)));
            float y1=temp_bottom_height;
            float z1=(float) (temp_bottom_radius*sin(toRadians(hAngle)));
            //当前四边形右上点X、Y、Z坐标
            float x2=(float) (temp_top_radius*cos(toRadians(hAngle+longitude_span)));
            float y2=temp_top_height;
            float z2=(float) (temp_top_radius*sin(toRadians(hAngle+longitude_span)));
            //当前四边形右下点X、Y、Z坐标
            float x3=(float) (temp_bottom_radius*cos(toRadians(hAngle+longitude_span)));
            float y3=temp_bottom_height;
            float z3=(float) (temp_bottom_radius*sin(toRadians(hAngle+longitude_span)));
            //将顶点坐标按照卷绕成两个三角形的顺序依次放入顶点坐标列表
            alVertix.push_back(x0);alVertix.push_back(y0);alVertix.push_back(z0);
            alVertix.push_back(x1);alVertix.push_back(y1);alVertix.push_back(z1);
            alVertix.push_back(x2);alVertix.push_back(y2);alVertix.push_back(z2);

            alVertix.push_back(x2);alVertix.push_back(y2);alVertix.push_back(z2);
            alVertix.push_back(x1);alVertix.push_back(y1);alVertix.push_back(z1);
            alVertix.push_back(x3);alVertix.push_back(y3);alVertix.push_back(z3);
        }
        //创建纹理数据
        std::vector<float> texcoor=generateTexCoor(
                        (int)(360/longitude_span), //纹理图切分的列数
                        1
        );
        for(int i=0;i<360;i++){
            alTexCoor.push_back(texcoor[i]);
        }
    }
    vCount=alVertix.size()/3;//得到顶点数量
    dataByteCount = vCount * 5 * sizeof(float);//计算数据所占字节数
    vdata = new float[vCount*5];//创建存放数据的数组
    int index = 0;//辅助索引
    for (int i = 0; i < vCount; i++) {//将顶点数据和纹理坐标数据存储到数组中
        vdata[index++] = alVertix[i * 3 + 0];
        vdata[index++] = alVertix[i * 3 + 1];
        vdata[index++] = alVertix[i * 3 + 2];
        vdata[index++] = alTexCoor[i * 2 + 0];
        vdata[index++] = alTexCoor[i * 2 + 1];
    }
}
