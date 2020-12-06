#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

//目前的实验其表明 必须是4的整数倍 否则奇怪现象
layout (std140,set = 0, binding = 0) uniform bufferVals {
	vec4 lightPosition;
	float near;
	float far;
} myBufferVals;

layout (location = 0) in vec4 vPosition;

layout (location = 0) out vec4 outColor;

//将near到far范围内的距离压缩到0～1之间
//压缩范围的好处是提高比较的精度
float fromDisToZ(float dis,float near,float far)
{
   return (dis-near)/(far-near);
}

//将距离值分到四个通道存储
vec4 from1To4(float total)
{
    vec4 result;
    result.r=mod(total,256.0)/256.0;
    result.g=mod(total/256.0,256.0)/256.0;
    result.b=mod(total/65536.0,256.0)/256.0;
    result.a=mod(total/16777216.0,256.0)/256.0;
    return result;
}

void main() {
    //这里采用的near=1.0 far=1000.0要与实际的光源处投影矩阵相关参数一致
    //计算当前片元距离光源的距离，并且将距离规约到0～1之间
    float disNormal=fromDisToZ(distance(myBufferVals.lightPosition.xyz,vPosition.xyz),myBufferVals.near,myBufferVals.far);
    //将距离扩大到0~2^32
    float total=disNormal*4294967296.0;
    //将距离分到四个通道存储
    outColor=from1To4(total);
}