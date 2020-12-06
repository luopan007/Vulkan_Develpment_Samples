#include "HelpFunction.h"
#include <vector>
bool memoryTypeFromProperties(VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t typeBits,VkFlags requirements_mask,uint32_t *typeIndex)
{
    for (uint32_t i = 0; i < 32; i++)
    {
        if ((typeBits & 1) == 1)
        {
            if ((memoryProperties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask)
            {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    return false;
}
float toRadians(float degree){//角度转换成弧度的方法
    return degree*3.1415926535898/180;
}
float toDegrees(float radian){//弧度变换成角度的方法
    return radian*180/3.1415926535898;
}
//自动切分纹理的方法
std::vector<float> generateTexCoor(int bw,int bh){
        std::vector<float> result;
        float sizew=1.0f/bw;//列数
        float sizeh=1.0f/bh;//行数
        for(int i=0;i<bh;i++)
        {
            for(int j=0;j<bw;j++)
            {
                //每行列一个矩形，由两个三角形构成，共六个点，12个纹理坐标
                float s=j*sizew;
                float t=i*sizeh;

                result.push_back(s);
                result.push_back(t);

                result.push_back(s);
                result.push_back(t+sizeh);

                result.push_back(s+sizew);
                result.push_back(t);

                result.push_back(s+sizew);
                result.push_back(t);

                result.push_back(s);
                result.push_back(t+sizeh);

                result.push_back(s+sizew);
                result.push_back(t+sizeh);
            }
        }
        return result;
}
std::vector<float> generateSkyTexCoor(int bw,int bh){
    std::vector<float> result;
    float sizew=1.0f/bw;//列数
    float sizeh=1.0f/bh;//行数
    for(int i=0;i<bh;i++)
    {
        for(int j=0;j<bw;j++)
        {
            //每行列一个矩形，由两个三角形构成，共六个点，12个纹理坐标
            float s=j*sizew;
            float t=i*sizeh;

            result.push_back(s);
            result.push_back(t);

            result.push_back(s+sizew);
            result.push_back(t);

            result.push_back(s);
            result.push_back(t+sizeh);

            result.push_back(s);
            result.push_back(t+sizeh);

            result.push_back(s+sizew);
            result.push_back(t);

            result.push_back(s+sizew);
            result.push_back(t+sizeh);
        }
    }
    return result;
}