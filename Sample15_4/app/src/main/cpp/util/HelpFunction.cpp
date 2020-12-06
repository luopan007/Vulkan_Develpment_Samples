#include <vector>
#include "HelpFunction.h"
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
float toRadians(float degree){
    return degree*3.1415926535898/180;
}
std::vector<float> generateSkyTexCoor(int bw,int bh){
    std::vector<float> result;
    float sizew=1.0f/bw;
    float sizeh=1.0f/bh;
    for(int i=0;i<bh;i++)
    {
        for(int j=0;j<bw;j++)
        {
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