#include "LoadNormalUtil.h"
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
size_t  LoadNormalUtil::splitString(const string& strSrc, const string& strDelims, vector<string>& strDest)
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
bool LoadNormalUtil::tryParseDouble(const char *s, const char *s_end, double *result)
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
float LoadNormalUtil::parseFloat(const char* token)
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
int LoadNormalUtil::parseInt(const char *token) {
    token += strspn(token, " \t");
    int i = atoi(token);
    return i;
}
AAssetManager* LoadNormalUtil::aam;
void LoadNormalUtil::setAAssetManager(AAssetManager* aamIn)
{
    aam=aamIn;
}
float* LoadNormalUtil::getCrossProduct(float x1,float y1,float z1,float x2,float y2,float z2)
{
    float A=y1 * z2 - y2 * z1;
    float B=z1 * x2 - z2 * x1;
    float C=x1 * y2 - x2 * y1;
    float *cProduct=new float[3]{A,B,C};
    return cProduct;
}
float* LoadNormalUtil::vectorNormal(float *vector)
{
    float module = (float)sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);
    float *vNormal=new float[3]{vector[0]/module,vector[1]/module,vector[2]/module};
    return vNormal;
}
ObjObject* LoadNormalUtil::loadFromFile(const string& vname,VkDevice& device,VkPhysicalDeviceMemoryProperties& memoryroperties)
{
    ObjObject* lo;
    vector<float> alv;
    vector<float> alvResult;
    vector<float> alnResult;
    std::string resultStr=FileUtil::loadAssetStr(vname);
    vector<string> lines;
    LoadNormalUtil::splitString(resultStr, "\n", lines);
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
        LoadNormalUtil::splitString(tempContents,delims, splitStrs);
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
            for(int i=0;i<3;i++)
            {
                alnResult.push_back(vNormal[0]);
                alnResult.push_back(vNormal[1]);
                alnResult.push_back(vNormal[2]);
            }
        }
        splitStrs.clear();
    }
    int vCount=(int)alvResult.size()/3;
    int dataByteCount=vCount*6*sizeof(float);
    float* vdataIn=new float[vCount*6];
    int indexTemp=0;
    for(int i=0;i<vCount;i++)
    {
        vdataIn[indexTemp++]=alvResult[i*3+0];
        vdataIn[indexTemp++]=alvResult[i*3+1];
        vdataIn[indexTemp++]=alvResult[i*3+2];
        vdataIn[indexTemp++]=alnResult[i*3+0];
        vdataIn[indexTemp++]=alnResult[i*3+1];
        vdataIn[indexTemp++]=alnResult[i*3+2];
    }
    lo=new ObjObject(vdataIn,dataByteCount,vCount,device, memoryroperties);
    return lo;
}
