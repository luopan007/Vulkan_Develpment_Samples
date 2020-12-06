#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140,set = 0, binding = 1) uniform bufferValsFrag { //一致块
mat4 uMVPMatrixMirror; //镜像摄像机投影与观察组合矩阵
} myBufferValsFrag;
layout (binding = 2) uniform sampler2D texDY; //纹理采样器（镜像，即倒影）
layout (binding = 3) uniform sampler2D texSelf; //纹理采样器（水面自身纹理）
layout (binding = 4) uniform sampler2D texNormal; //纹理采样器（法向量纹理）
layout (location = 0) in vec2 inTexCoor; //顶点着色器传入的纹理坐标
layout (location = 1) in vec4 inLightQD; //顶点着色器传入的光照强度
layout (location = 2) in vec4 vPosition; //顶点着色器传入的顶点位置
layout (location = 3) in vec3 vNormal; //顶点着色器传入的顶点法向量
layout (location = 0) out vec4 outColor; //片元最终颜色值
void main(){ //主函数
vec4 gytyPosition= //将片元位置通过矩阵变换进剪裁空间中
myBufferValsFrag.uMVPMatrixMirror * vec4(vPosition.xyz,1.0);
gytyPosition=gytyPosition/gytyPosition.w; //执行透视除法将片元位置变换进标准设备空间中
float s=(gytyPosition.x+1.0)/2.0; //将标准设备空间中XOY 平面内的X 坐标变换为纹理S 坐标
float t=(gytyPosition.y+1.0)/2.0; //将标准设备空间中XOY 平面内的Y 坐标变换为纹理T 坐标
vec4 normalColor = textureLod(texNormal, inTexCoor,0.0); //从法线纹理图中读出值
vec3 cNormal= //将值恢复到-1～+1 范围内
vec3(2.0*(normalColor.r-0.5),2.0*(normalColor.g-0.5),2.0*(normalColor.b-0.5));
cNormal=normalize(cNormal); //将扰动结果向量规格化
const float mPerturbationAmt=0.02f; //扰动系数(控制扭曲程度用)
s=s+mPerturbationAmt*s*cNormal.x; //计算扰动后的纹理坐标S
t=t+mPerturbationAmt*t*cNormal.y; //计算扰动后的纹理坐标T
vec4 dyColor=textureLod(texDY, vec2(s,t),0.0); //进行倒影纹理采样(投影贴图)
vec4 waterColor=textureLod(texSelf,inTexCoor,0.0); //进行水面自身纹理采样
//混合倒影与水面自身得到此片元的颜色值，倒影纹理占50%，水面自身纹理占50%
vec4 dyAndWaterColor=mix(waterColor,dyColor,0.50);
outColor=dyAndWaterColor*inLightQD; //结合光照强度计算出片元最终颜色值
}