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
ObjObject* LoadUtil::loadFromFile(const string& vname,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties){//读取obj 文件内容生成绘制用物体对象的方法
    ObjObject * lo; //指向生成的绘制用物体对象的指针
    vector<float> alv;
    vector<float> alvResult;
    vector<float> aln; //存放原始法向量数据的列表
    vector<float>alnResult; //存放结果法向量数据的列表
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
        else if(splitStrs[0]=="vn")
        { //若此行为vn 开头则为法向量数据行
            aln.push_back(parseFloat(splitStrs[1].c_str())); //将法向量X 分量存入原始法向量列表
            aln.push_back(parseFloat(splitStrs[2].c_str())); //将法向量Y 分量存入原始法向量列表
            aln.push_back(parseFloat(splitStrs[3].c_str())); //将法向量Z 分量存入原始法向量列表
        }else if(splitStrs[0]=="f")
        { //若此行为f 开头则为面数据行
            int index[3];
            string delimsF ="/";
            splitStrsF.clear();
            splitString(splitStrs[1].c_str(),delimsF,splitStrsF);
            index[0]=parseInt(splitStrsF[0].c_str())-1;
            alvResult.push_back(alv[3*index[0]]);
            alvResult.push_back(alv[3*index[0]+1]);
            alvResult.push_back(alv[3*index[0]+2]);
            int indexN=parseInt(splitStrsF[2].c_str())-1;//获取三角形面第1 个顶点的法向量编号
            alnResult.push_back(aln[3*indexN]); //将第1 个顶点法向量的x 分量存入结果法向量列表
            alnResult.push_back(aln[3*indexN+1]); //将第1 个顶点法向量的y 分量存入结果法向量列表
            alnResult.push_back(aln[3*indexN+2]); //将第1 个顶点法向量的z 分量存入结果法向量列表
            splitStrsF.clear();
            splitString(splitStrs[2].c_str(),delimsF,splitStrsF);
            index[1]=parseInt(splitStrsF[0].c_str())-1;
            alvResult.push_back(alv[3*index[1]]);
            alvResult.push_back(alv[3*index[1]+1]);
            alvResult.push_back(alv[3*index[1]+2]);
            indexN=parseInt(splitStrsF[2].c_str())-1;//获取三角形面第2 个顶点的法向量编号
            alnResult.push_back(aln[3*indexN]); //将第2 个顶点法向量的x 分量存入结果法向量列表
            alnResult.push_back(aln[3*indexN+1]); //将第2 个顶点法向量的y 分量存入结果法向量列表
            alnResult.push_back(aln[3*indexN+2]); //将第2 个顶点法向量的z 分量存入结果法向量列表
            splitStrsF.clear();
            splitString(splitStrs[3].c_str(),delimsF,splitStrsF);
            index[2]=parseInt(splitStrsF[0].c_str())-1;
            alvResult.push_back(alv[3*index[2]]);
            alvResult.push_back(alv[3*index[2]+1]);
            alvResult.push_back(alv[3*index[2]+2]);
            indexN=parseInt(splitStrsF[2].c_str())-1;//获取三角形面第3 个顶点的法向量编号
            alnResult.push_back(aln[3*indexN]); //将第3 个顶点法向量的x 分量存入结果法向量列表
            alnResult.push_back(aln[3*indexN+1]); //将第3 个顶点法向量的y 分量存入结果法向量列表
            alnResult.push_back(aln[3*indexN+2]); //将第3 个顶点法向量的z 分量存入结果法向量列表
        }
        splitStrs.clear();
    }
    int vCount=(int)alvResult.size()/3; //计算顶点数量
    int dataByteCount=vCount*6*sizeof(float); //计算顶点数据所占总字节数
    float* vdataIn=new float[vCount*6]; //创建顶点数据数组
    int indexTemp=0; //辅助索引
    for(int i=0;i<vCount;i++){ //遍历每个顶点
        vdataIn[indexTemp++]=alvResult[i*3+0];
        vdataIn[indexTemp++]=alvResult[i*3+1];
        vdataIn[indexTemp++]=alvResult[i*3+2];
        vdataIn[indexTemp++]=alnResult[i*3+0]; //将法向量x 分量转存到顶点数据数组中
        vdataIn[indexTemp++]=alnResult[i*3+1]; //将法向量y 分量转存到顶点数据数组中
        vdataIn[indexTemp++]=alnResult[i*3+2]; //将法向量z 分量转存到顶点数据数组中
    }
    lo=new ObjObject(vdataIn,dataByteCount,vCount,device, memoryroperties);//创建绘制用物体对象
    return lo; //返回指向绘制用物体对象的指针
}

