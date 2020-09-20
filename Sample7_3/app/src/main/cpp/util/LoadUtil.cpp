#include "LoadUtil.h"
#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "FileUtil.h"
#include "Normal.h"
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
float* getCrossProduct(float x1,float y1,float z1,float x2,float y2,float z2)
{
    float A=y1 * z2 - y2 * z1;
    float B=z1 * x2 - z2 * x1;
    float C=x1 * y2 - x2 * y1;
    float *cProduct=new float[3]{A,B,C};
    return cProduct;
}
float* LoadUtil::vectorNormal(float *vector)
{
    float module = (float)sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);
    float *vNormal=new float[3]{vector[0]/module,vector[1]/module,vector[2]/module};
    return vNormal;
}
ObjObject* LoadUtil::loadFromFile(const string& vname,VkDevice& device,
                                  VkPhysicalDeviceMemoryProperties& memoryroperties){//读取obj 文件内容生成绘制用物体对象的方法
    ObjObject* lo; //指向生成的绘制用物体对象的指针
    vector<float> alv; //存放原始顶点坐标数据的列表
    vector<float> alvResult; //存放结果顶点坐标数据的列表
    vector<int> alFaceIndex; //存放三角形面顶点编号的列表
    map<int,set<Normal*>> hmn; //存放各顶点法向量列表的map

    std::string resultStr=FileUtil::loadAssetStr(vname);
    vector<string> lines;
    splitString(resultStr, "\n", lines);
    vector<string> splitStrs;
    vector<string> splitStrsF;
    string tempContents;
    for(int i=0;i<lines.size();i++)
    {
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
            float vxa=x1-x0;
            float vya=y1-y0;
            float vza=z1-z0;
            float vxb=x2-x0;
            float vyb=y2-y0;
            float vzb=z2-z0;
            float *vNormal =vectorNormal(getCrossProduct(vxa,vya,vza,vxb,vyb,vzb));
            alFaceIndex.push_back(index[0]); //记录三角形面第一个顶点的编号
            alFaceIndex.push_back(index[1]); //记录三角形面第二个顶点的编号
            alFaceIndex.push_back(index[2]); //记录三角形面第三个顶点的编号
            for (int tempIndex : index){//将此三角形面的法向量记录到此面3 个顶点各自的法向量集合中
                set <Normal*> setN= hmn[tempIndex]; //由顶点编号获取对应的法向量集合
                Normal *normal =new Normal(vNormal[0], vNormal[1], vNormal[2]); //创建法向量对象
                if(!Normal::exist(normal, setN)){ //判断当前法向量是否不在当前点的法向量集合中
                    setN.insert(normal); //若不在，则将该法向量添加到当前点的法向量集合中
                }
                hmn[tempIndex] = setN; //更新map 中当前点的法向量集合
            }}splitStrs.clear();
    }
    int vCount = (int)alvResult.size() / 3; //计算顶点数量
    int dataByteCount = vCount * 6 * sizeof(float); //计算顶点数据所占总字节数
    float* vdataIn = new float[vCount * 6]; //创建顶点数据数组
    set<Normal*> setNTemp; //存放一个顶点法向量集合的辅助变量
    float *nTemp; //指向存放向量三分量数据数组的指针
    int indexTemp = 0; //辅助索引
    for (int i = 0; i<vCount; i++){ //遍历所有的顶点
        vdataIn[indexTemp++] = alvResult[i * 3 + 0]; //将顶点x 坐标转存到顶点数据数组中
        vdataIn[indexTemp++] = alvResult[i * 3 + 1]; //将顶点y 坐标转存到顶点数据数组中
        vdataIn[indexTemp++] = alvResult[i * 3 + 2]; //将顶点z 坐标转存到顶点数据数组中
        setNTemp= (hmn[alFaceIndex.at(i)]); //获取当前顶点的法向量集合
        nTemp=Normal::getAverage(setNTemp); //求出此顶点的平均法向量
        vdataIn[indexTemp++] = nTemp[0]; //将平均法向量的x 分量转存到顶点数据数组中
        vdataIn[indexTemp++] = nTemp[1]; //将平均法向量的y 分量转存到顶点数据数组中
        vdataIn[indexTemp++] = nTemp[2]; //将平均法向量的z 分量转存到顶点数据数组中
    }
    lo = new ObjObject(vdataIn, dataByteCount, vCount, device, memoryroperties); //创建绘制用物体对象
    return lo; //返回指向绘制用物体对象的指针
}


