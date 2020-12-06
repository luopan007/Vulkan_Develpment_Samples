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
float toRadians(float degree) {//角度转换成弧度的方法
    return degree*3.1415926535898f / 180.0f;
}
float toDegrees(float radian) {//弧度变换成角度的方法
    return radian * 180.0f / 3.1415926535898f;
}
