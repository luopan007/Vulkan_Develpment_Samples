#include "LoadUtil.h"
#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "../bndev/mylog.h"
#include <vulkan/vulkan.h>
#include "FileUtil.h"
using namespace std;
size_t  splitString(const string& strSrc, const string& strDelims, vector<string>& strDest)
{
    string delims = strDelims;
    string STR;
    if(delims.empty()) delims = " **";
    string::size_type pos=0;
    string::size_type LEN = strSrc.size();
    while(pos < LEN ){
        STR="";
        while( (delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN) )
        {
            ++pos;
        }
        if(pos==LEN) {
            return strDest.size();
        }
        while( (delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN) )
        {
            STR += strSrc[pos++];
        }
        if( ! STR.empty() )
        {
            strDest.push_back(STR);
        }
    }
    return strDest.size();
}
bool tryParseDouble(const char *s, const char *s_end, double *result)
{
    if (s >= s_end)
    {
        return false;
    }
    double mantissa = 0.0;
    int exponent = 0;
    char sign = '+';
    char exp_sign = '+';
    char const *curr = s;
    int read = 0;
    bool end_not_reached = false;
    if (*curr == '+' || *curr == '-')
    {
        sign = *curr;
        curr++;
    }
    else if (isdigit(*curr)) { /* Pass through. */ }
    else
    {
        goto fail;
    }
    while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
    {
        mantissa *= 10;
        mantissa += static_cast<int>(*curr - 0x30);
        curr++;	read++;
    }
    if (read == 0)
        goto fail;
    if (!end_not_reached)
        goto assemble;
    if (*curr == '.')
    {
        curr++;
        read = 1;
        while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
        {
            mantissa += static_cast<int>(*curr - 0x30) * pow(10.0, -read);
            read++; curr++;
        }
    }
    else if (*curr == 'e' || *curr == 'E') {}
    else
    {
        goto assemble;
    }
    if (!end_not_reached)
        goto assemble;
    if (*curr == 'e' || *curr == 'E')
    {
        curr++;
        if ((end_not_reached = (curr != s_end)) && (*curr == '+' || *curr == '-'))
        {
            exp_sign = *curr;
            curr++;
        }
        else if (isdigit(*curr)) { /* Pass through. */ }
        else
        {
            goto fail;
        }
        read = 0;
        while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
        {
            exponent *= 10;
            exponent += static_cast<int>(*curr - 0x30);
            curr++;	read++;
        }
        exponent *= (exp_sign == '+'? 1 : -1);
        if (read == 0)
            goto fail;
    }
    assemble:
    *result = (sign == '+'? 1 : -1) * ldexp(mantissa * pow(5.0, exponent), exponent);
    return true;
    fail:
    return false;
}
float parseFloat(const char* token)
{
    token += strspn(token, " \t");
#ifdef TINY_OBJ_LOADER_OLD_FLOAT_PARSER
    float f = atof(token);
#else
    const char *end = token + strcspn(token, " \t\r");
    double val = 0.0;
    tryParseDouble(token, end, &val);
    float f = static_cast<float>(val);
#endif
    return f;
}
int parseInt(const char *token) {
    token += strspn(token, " \t");
    int i = atoi(token);
    return i;
}
float* getCrossProduct(float x1,float y1,float z1,float x2,float y2,float z2){ //求两个向量叉积的方法
    float A=y1 * z2 - y2 * z1; //求出两个向量的叉积向量在X 轴的分量A
    float B=z1 * x2 - z2 * x1; //求出两个向量的叉积向量在Y 轴的分量B
    float C=x1 * y2 - x2 * y1; //求出两个向量的叉积向量在Z 轴的分量C
    return new float[3]{A,B,C}; //返回叉积结果向量
}
float* vectorNormal(float *vector){ //向量规格化的方法
    float module = (float)sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]); //求向量的模
    return new float[3]{vector[0]/module,vector[1]/module,vector[2]/module}; //返回规格化的向量
}

ObjObject* LoadUtil::loadFromFile(const string& vname,VkDevice& device,
                                  VkPhysicalDeviceMemoryProperties& memoryroperties){ //读取obj 文件内容生成绘制用物体对象的方法
    ObjObject* lo; //指向生成的绘制用物体对象的指针
    vector<float> alv; //存放原始顶点坐标数据的列表
    vector<float> alvResult; //存放结果顶点坐标数据的列表
    vector<float> alnResult; //存放结果法向量数据的列表
    std::string resultStr=FileUtil::loadAssetStr(vname);
    vector<string> lines;
    splitString(resultStr, "\n", lines);
    vector<string> splitStrs;
    vector<string> splitStrsF;
    string tempContents;
    for(int i=0;i<lines.size();i++){ //遍历obj 文件中每行的字符串
        tempContents=lines[i];
        if(tempContents.compare("")==0)
        {
            continue;
        }
        string delims ="[ ]+";
        splitStrs.clear();
        splitString(tempContents,delims, splitStrs);
        if(splitStrs[0]=="v")
        {
            alv.push_back(parseFloat(splitStrs[1].c_str()));
            alv.push_back(parseFloat(splitStrs[2].c_str()));
            alv.push_back(parseFloat(splitStrs[3].c_str()));
        }
        else if(splitStrs[0]=="f")
        {
            int index[3];
            string delimsF ="/";
            splitStrsF.clear();
            splitString(splitStrs[1].c_str(),delimsF,splitStrsF);
            index[0]=parseInt(splitStrsF[0].c_str())-1;
            float x0=alv[3*index[0]];
            float y0=alv[3*index[0]+1];
            float z0=alv[3*index[0]+2];
            alvResult.push_back(x0);
            alvResult.push_back(y0);
            alvResult.push_back(z0);
            splitStrsF.clear();
            splitString(splitStrs[2].c_str(),delimsF,splitStrsF);
            index[1]=parseInt(splitStrsF[0].c_str())-1;
            float x1=alv[3*index[1]];
            float y1=alv[3*index[1]+1];
            float z1=alv[3*index[1]+2];
            alvResult.push_back(x1);
            alvResult.push_back(y1);
            alvResult.push_back(z1);
            splitStrsF.clear();
            splitString(splitStrs[3].c_str(),delimsF,splitStrsF);
            index[2]=parseInt(splitStrsF[0].c_str())-1;
            float x2=alv[3*index[2]];
            float y2=alv[3*index[2]+1];
            float z2=alv[3*index[2]+2];
            alvResult.push_back(x2);
            alvResult.push_back(y2);
            alvResult.push_back(z2);
            float vxa=x1-x0; float vya=y1-y0; float vza=z1-z0; //求三角形中第一个点到第二个点的向量
            float vxb=x2-x0; float vyb=y2-y0; float vzb=z2-z0; //求三角形中第一个点到第三个点的向量
            float *vNormal = //通过计算两个向量的叉积计算出此三角形面的法向量
                    vectorNormal(getCrossProduct(vxa,vya,vza,vxb,vyb,vzb));
            for(int i=0;i<3;i++){ //遍历当前三角形的三个顶点
                alnResult.push_back(vNormal[0]);//将计算出的法向量X 分量添加到结果法向量列表中
                alnResult.push_back(vNormal[1]);//将计算出的法向量Y 分量添加到结果法向量列表中
                alnResult.push_back(vNormal[2]);//将计算出的法向量Z 分量添加到结果法向量列表中
            }}splitStrs.clear();}
    int vCount=(int)alvResult.size()/3; //计算顶点数量
    int dataByteCount=vCount*6*sizeof(float); //技术顶点数据所占总字节数
    float* vdataIn=new float[vCount*6]; //顶点数据数组
    int indexTemp=0; //辅助索引
    for(int i=0;i<vCount;i++){ //遍历所有的顶点
        vdataIn[indexTemp++]=alvResult[i*3+0]; //将当前顶点x 坐标转存到顶点数据数组中
        vdataIn[indexTemp++]=alvResult[i*3+1]; //将当前顶点y 坐标转存到顶点数据数组中
        vdataIn[indexTemp++]=alvResult[i*3+2]; //将当前顶点z 坐标转存到顶点数据数组中
        vdataIn[indexTemp++]=alnResult[i*3+0]; //将面法向量x 分量转存到顶点数据数组中
        vdataIn[indexTemp++]=alnResult[i*3+1]; //将面法向量y 分量转存到顶点数据数组中
        vdataIn[indexTemp++]=alnResult[i*3+2]; //将面法向量z 分量转存到顶点数据数组中
    }
    lo=new ObjObject(vdataIn,dataByteCount,vCount,device, memoryroperties);//创建绘制用物体对象
    return lo; //返回指向绘制用物体对象的指针
}

