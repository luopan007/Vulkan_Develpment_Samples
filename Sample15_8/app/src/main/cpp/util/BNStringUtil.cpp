#include "BNStringUtil.h"
size_t  splitString(const string& strSrc, const string& strDelims, vector<string>& strDest)
{
    string delims = strDelims;
    string STR;
    if (delims.empty()) delims = " **";
    string::size_type pos = 0;
    string::size_type LEN = strSrc.size();
    while (pos < LEN) {
        STR = "";
        while ((delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN))
        {
            ++pos;
        }
        if (pos == LEN) {
            return strDest.size();
        }
        while ((delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN))
        {
            STR += strSrc[pos++];
        }
        if (!STR.empty())
        {
            strDest.push_back(STR);
        }
    }
    return strDest.size();
}