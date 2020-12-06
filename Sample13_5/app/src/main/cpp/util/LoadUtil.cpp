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
#include "ObjObjectUt.h"
#include "QKJUtil.h"

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
 ObjObject* LoadUtil::loadFromFile(const string& vname,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties){
    ObjObject * lo; 
    vector<float> alv;
    vector<float> alvResult;
    vector<float> alt;
    vector<float> altResult;
    vector<float> aln; 
    vector<float>alnResult; 
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
        else if(splitStrs[0]=="vt")
        {
            alt.push_back(parseFloat(splitStrs[1].c_str()));
            alt.push_back(1-parseFloat(splitStrs[2].c_str()));
        }
        else if(splitStrs[0]=="vn")
        { 
            aln.push_back(parseFloat(splitStrs[1].c_str())); 
            aln.push_back(parseFloat(splitStrs[2].c_str())); 
            aln.push_back(parseFloat(splitStrs[3].c_str())); 
        }else if(splitStrs[0]=="f")
        { 
            int index[3];
            string delimsF ="/";
            splitStrsF.clear();
            splitString(splitStrs[1].c_str(),delimsF,splitStrsF);
            index[0]=parseInt(splitStrsF[0].c_str())-1;
            alvResult.push_back(alv[3*index[0]]);
            alvResult.push_back(alv[3*index[0]+1]);
            alvResult.push_back(alv[3*index[0]+2]);
            int indexTex=parseInt(splitStrsF[1].c_str())-1;
            altResult.push_back(alt[indexTex*2]);
            altResult.push_back(alt[indexTex*2+1]);
            int indexN=parseInt(splitStrsF[2].c_str())-1;
            alnResult.push_back(aln[3*indexN]); 
            alnResult.push_back(aln[3*indexN+1]); 
            alnResult.push_back(aln[3*indexN+2]); 
            splitStrsF.clear();
            splitString(splitStrs[2].c_str(),delimsF,splitStrsF);
            index[1]=parseInt(splitStrsF[0].c_str())-1;
            alvResult.push_back(alv[3*index[1]]);
            alvResult.push_back(alv[3*index[1]+1]);
            alvResult.push_back(alv[3*index[1]+2]);
            indexTex=parseInt(splitStrsF[1].c_str())-1;
            altResult.push_back(alt[indexTex*2]);
            altResult.push_back(alt[indexTex*2+1]);
            indexN=parseInt(splitStrsF[2].c_str())-1;
            alnResult.push_back(aln[3*indexN]); 
            alnResult.push_back(aln[3*indexN+1]); 
            alnResult.push_back(aln[3*indexN+2]); 
            splitStrsF.clear();
            splitString(splitStrs[3].c_str(),delimsF,splitStrsF);
            index[2]=parseInt(splitStrsF[0].c_str())-1;
            alvResult.push_back(alv[3*index[2]]);
            alvResult.push_back(alv[3*index[2]+1]);
            alvResult.push_back(alv[3*index[2]+2]);
            indexTex=parseInt(splitStrsF[1].c_str())-1;
            altResult.push_back(alt[indexTex*2]);
            altResult.push_back(alt[indexTex*2+1]);
            indexN=parseInt(splitStrsF[2].c_str())-1;
            alnResult.push_back(aln[3*indexN]); 
            alnResult.push_back(aln[3*indexN+1]); 
            alnResult.push_back(aln[3*indexN+2]); 
        }
        splitStrs.clear();
    }
     int vCount=(int)alvResult.size()/3; 
     int dataByteCount=vCount*8*sizeof(float); 
     float* vdataIn=new float[vCount*8]; 
     int indexTemp=0; 
     for(int i=0;i<vCount;i++){ 
         vdataIn[indexTemp++]=alvResult[i*3+0];
         vdataIn[indexTemp++]=alvResult[i*3+1];
         vdataIn[indexTemp++]=alvResult[i*3+2];
         vdataIn[indexTemp++]=altResult[i*2+0];
         vdataIn[indexTemp++]=altResult[i*2+1];
         vdataIn[indexTemp++]=alnResult[i*3+0]; 
         vdataIn[indexTemp++]=alnResult[i*3+1]; 
         vdataIn[indexTemp++]=alnResult[i*3+2]; 
     }
     lo=new ObjObject(vdataIn,dataByteCount,vCount,device, memoryroperties);
     return lo; 
 }
ObjObjectUt* LoadUtil::loadFromFileUt(const string& vname,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties){
    ObjObjectUt * lo;
    vector<float> alv;
    vector<float> alvResult;
    vector<float> alt;
    vector<float> altResult;
    vector<float> aln;
    vector<float>alnResult;
    std::string resultStr = FileUtil::loadAssetStr(vname);
    vector<string> lines;
    splitString(resultStr, "\n", lines);
    vector<string> splitStrs;
    vector<string> splitStrsF;
    string tempContents;
    for (int i = 0; i<lines.size(); i++)
    {
        tempContents = lines[i];
        if (tempContents.compare("") == 0)
        {
            continue;
        }
        string delims = "[ ]+";
        splitStrs.clear();
        splitString(tempContents, delims, splitStrs);
        if (splitStrs[0] == "v")
        {
            alv.push_back(parseFloat(splitStrs[1].c_str()));
            alv.push_back(parseFloat(splitStrs[2].c_str()));
            alv.push_back(parseFloat(splitStrs[3].c_str()));
        }
        else if (splitStrs[0] == "vt")
        {
            alt.push_back(parseFloat(splitStrs[1].c_str()));
            alt.push_back(1 - parseFloat(splitStrs[2].c_str()));
        }
        else if (splitStrs[0] == "vn")
        {
            aln.push_back(parseFloat(splitStrs[1].c_str()));
            aln.push_back(parseFloat(splitStrs[2].c_str()));
            aln.push_back(parseFloat(splitStrs[3].c_str()));
        }
        else if (splitStrs[0] == "f")
        {
            int index[3];
            string delimsF = "/";
            splitStrsF.clear();
            splitString(splitStrs[1].c_str(), delimsF, splitStrsF);
            index[0] = parseInt(splitStrsF[0].c_str()) - 1;
            alvResult.push_back(alv[3 * index[0]]);
            alvResult.push_back(alv[3 * index[0] + 1]);
            alvResult.push_back(alv[3 * index[0] + 2]);
            int indexTex = parseInt(splitStrsF[1].c_str()) - 1;
            altResult.push_back(alt[indexTex * 2]);
            altResult.push_back(alt[indexTex * 2 + 1]);
            int indexN = parseInt(splitStrsF[2].c_str()) - 1;
            alnResult.push_back(aln[3 * indexN]);
            alnResult.push_back(aln[3 * indexN + 1]);
            alnResult.push_back(aln[3 * indexN + 2]);
            splitStrsF.clear();
            splitString(splitStrs[2].c_str(), delimsF, splitStrsF);
            index[1] = parseInt(splitStrsF[0].c_str()) - 1;
            alvResult.push_back(alv[3 * index[1]]);
            alvResult.push_back(alv[3 * index[1] + 1]);
            alvResult.push_back(alv[3 * index[1] + 2]);
            indexTex = parseInt(splitStrsF[1].c_str()) - 1;
            altResult.push_back(alt[indexTex * 2]);
            altResult.push_back(alt[indexTex * 2 + 1]);
            indexN = parseInt(splitStrsF[2].c_str()) - 1;
            alnResult.push_back(aln[3 * indexN]);
            alnResult.push_back(aln[3 * indexN + 1]);
            alnResult.push_back(aln[3 * indexN + 2]);
            splitStrsF.clear();
            splitString(splitStrs[3].c_str(), delimsF, splitStrsF);
            index[2] = parseInt(splitStrsF[0].c_str()) - 1;
            alvResult.push_back(alv[3 * index[2]]);
            alvResult.push_back(alv[3 * index[2] + 1]);
            alvResult.push_back(alv[3 * index[2] + 2]);
            indexTex = parseInt(splitStrsF[1].c_str()) - 1;
            altResult.push_back(alt[indexTex * 2]);
            altResult.push_back(alt[indexTex * 2 + 1]);
            indexN = parseInt(splitStrsF[2].c_str()) - 1;
            alnResult.push_back(aln[3 * indexN]);
            alnResult.push_back(aln[3 * indexN + 1]);
            alnResult.push_back(aln[3 * indexN + 2]);
        }
        splitStrs.clear();
    }
    int vCount = (int)alvResult.size() / 3;
    int dataByteCount = vCount * 11 * sizeof(float);
    float* vdataIn = new float[vCount * 11];
    int indexTemp = 0;

    int triCount = alvResult.size() / (3 * 3); //总的切向量数量(以三角形数量计)
    vector<float> altnResult; //存放切向量的列表
    for (int i = 0; i<triCount; i++){ //遍历计算每个三角形的切向量
        //三角形面的三个顶点的位置坐标
        double p0x=alvResult[i * 9 + 0];double p0y=alvResult[i * 9 + 1];double p0z=alvResult[i * 9 + 2];
        double p1x=alvResult[i * 9 + 3];double p1y=alvResult[i * 9 + 4];double p1z=alvResult[i * 9 + 5];
        double p2x=alvResult[i * 9 + 6];double p2y=alvResult[i * 9 + 7];double p2z=alvResult[i * 9 + 8];
        double p0s=altResult[i * 6 + 0];double p0t=altResult[i * 6 + 1]; //第1 个顶点的纹理坐标
        double p1s=altResult[i * 6 + 2];double p1t=altResult[i * 6 + 3]; //第2 个顶点的纹理坐标
        double p2s=altResult[i * 6 + 4];double p2t=altResult[i * 6 + 5]; //第3 个顶点的纹理坐标
        vector<double> tangent = QKJUtil::calQKJ( //计算切向量
                p0x, p0y, p0z, //三角形面的第1 个点的位置坐标
                p1x, p1y, p1z, //三角形面的第2 个点的位置坐标
                p2x, p2y, p2z, //三角形面的第3 个点的位置坐标
                p0s, p0t, //三角形面的第1 个点的纹理坐标
                p1s, p1t, //三角形面的第2 个点的纹理坐标
                p2s, p2t //三角形面的第3 个点的纹理坐标
        );
        altnResult.push_back((float)tangent[0]);
        altnResult.push_back((float)tangent[1]);
        altnResult.push_back((float)tangent[2]);
        altnResult.push_back((float)tangent[0]);
        altnResult.push_back((float)tangent[1]);
        altnResult.push_back((float)tangent[2]);
        altnResult.push_back((float)tangent[0]);
        altnResult.push_back((float)tangent[1]);
        altnResult.push_back((float)tangent[2]);
    }

    for (int i = 0; i<vCount; i++) {
        vdataIn[indexTemp++] = alvResult[i * 3 + 0];
        vdataIn[indexTemp++] = alvResult[i * 3 + 1];
        vdataIn[indexTemp++] = alvResult[i * 3 + 2];

        vdataIn[indexTemp++] = altResult[i * 2 + 0];
        vdataIn[indexTemp++] = altResult[i * 2 + 1];

        vdataIn[indexTemp++] = alnResult[i * 3 + 0];
        vdataIn[indexTemp++] = alnResult[i * 3 + 1];
        vdataIn[indexTemp++] = alnResult[i * 3 + 2];

        vdataIn[indexTemp++] = altnResult[i * 3 + 0];//存入当前切向量的X 分量值
        vdataIn[indexTemp++] = altnResult[i * 3 + 1];//存入当前切向量的Y 分量值
        vdataIn[indexTemp++] = altnResult[i * 3 + 2];//存入当前切向量的Z 分量值
    }
    lo = new ObjObjectUt(vdataIn, dataByteCount, vCount, device, memoryroperties);
    return lo;
}

