#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex; //纹理采样器
layout (std140,set = 0, binding = 2) uniform bufferValsFrag { //一致变量块
mat4 uMVPMatrixGY; //光源处虚拟摄像机的投影与观察组合矩阵
} myBufferValsFrag;
layout (location = 0) in vec4 inLightQD; //从顶点着色器传入的最终光照强度
layout (location = 1) in vec4 vPosition; //从顶点着色器传入的顶点位置
layout (location = 2) in vec3 vNormal; //从顶点着色器传入的顶点法向量
layout (location = 3) in vec3 vlightPosition; //从顶点着色器传入的光源位置
layout (location = 0) out vec4 outColor; //输出到渲染管线的最终片元颜色值
void main(){
vec4 gytyPosition= //将片元位置通过矩阵变换进剪裁空间中
myBufferValsFrag.uMVPMatrixGY*vec4(vPosition.xyz,1.0);
gytyPosition=gytyPosition/gytyPosition.w; //执行透视除法将片元位置变换进标准设备空间中
float s=(gytyPosition.x+1.0)/2.0; //将标准设备空间中XOY 平面内的X 坐标变换为纹理S 坐标
float t=(gytyPosition.y+1.0)/2.0; //将标准设备空间中XOY 平面内的Y 坐标变换为纹理T 坐标
bool flag=dot (vNormal,vlightPosition-vPosition.xyz)>0?true:false;//基于法向量计算当前片元是否是正面朝向光源的
vec4 finalColor=vec4(1.0,1.0,1.0,1.0); //物体本身的颜色
if(s>=0.0&&s<=1.0&&t>=0.0&&t<=1.0&&flag){ //若纹理坐标在合法范围内则考虑投影贴图
vec4 projColor=textureLod(tex,vec2(s,t),0.0); //对投影纹理图进行采样
outColor=projColor*inLightQD; //结合光照强度计算出最终片元颜色值
}else{ //若纹理坐标不在合法范围内则不需要投影贴
outColor = finalColor*inLightQD; //结合光照强度计算出最终片元颜色值
}}
