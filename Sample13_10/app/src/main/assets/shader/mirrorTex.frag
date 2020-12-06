#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140,set = 0, binding = 0) uniform bufferValsFrag { //一致变量块
mat4 uMVPMatrixMirror; //镜像摄像机的投影与观察组合矩阵
} myBufferValsFrag;
layout (binding = 1) uniform sampler2D tex; //纹理采样器
layout (location = 0) in vec4 vPosition; //顶点着色器传入的顶点位置
layout (location = 0) out vec4 outColor; //输出到管线的片元颜色
void main(){ //主函数
vec4 gytyPosition= //将片元位置通过矩阵变换进剪裁空间中
myBufferValsFrag.uMVPMatrixMirror*vec4(vPosition.xyz,1.0);
gytyPosition=gytyPosition/gytyPosition.w; //执行透视除法将片元位置变换进标准设备空间中
float s=(gytyPosition.x+1.0)/2.0; //将标准设备空间中XOY 平面内的X 坐标变换为纹理S 坐标
float t=(gytyPosition.y+1.0)/2.0; //将标准设备空间中XOY 平面内的Y 坐标变换为纹理T 坐标
if(s>=0.0&&s<=1.0&&t>=0.0&&t<=1.0){ //若纹理坐标在合法范围内则考虑投影贴图
vec4 projColor=textureLod(tex,vec2(s,t),0.0); //对投影纹理图进行采样
outColor=projColor; //赋值给最终片元颜色值
}else{ //若纹理坐标不在合法范围内
outColor = vec4(1.0,1.0,1.0,1.0); //赋值给最终片元颜色值
}}