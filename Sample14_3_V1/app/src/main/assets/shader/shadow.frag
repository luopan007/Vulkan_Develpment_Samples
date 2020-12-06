#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致变量块
    vec4 lightPosition; //光源位置
    float near; //near 面距离
    float far; //far 面距离
} myBufferVals;
layout (location = 0) in vec4 vPosition; //接收从顶点着色器传递过来的片元位置
layout (location = 0) out vec4 outColor; //输出到管线的片元颜色
float fromDisToZ(float dis,float near,float far){ //将 near 到 far 范围内的距离压缩到 0～1 之间的方法
    return (dis-near)/(far-near);
}
vec4 from1To4(float total){ //将距离值分四个通道存储的方法
    vec4 result; //分四个通道存储的距离值
    result.r=mod(total,256.0)/256.0; //距离值对 256 取模再除以 256
    result.g=mod(total/256.0,256.0)/256.0; //距离值除以 2^8 并对 256 取模再除以 256
    result.b=mod(total/65536.0,256.0)/256.0; //距离值除以 2^16 并对 256 取模再除以 256
    result.a=mod(total/16777216.0,256.0)/256.0; //距离值除以 2^24 并对 256 取模再除以 256
    return result; //返回分四个通道存储的距离值
}
void main() { //主方法
    float disNormal=fromDisToZ(distance(myBufferVals.lightPosition.xyz,//计算最终距离值
    vPosition.xyz),myBufferVals.near,myBufferVals.far);
    float total=disNormal*4294967296.0; //将距离值扩大到 0~2^32
    outColor=from1To4(total); //将分四个通道存储的距离值输出
}
