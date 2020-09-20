#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D texC; //纹理采样器(基础颜色纹理)
layout (binding = 2) uniform sampler2D texD; //纹理采样器(过程纹理)
layout (binding = 3) uniform sampler2D texD1; //纹理采样器(细节纹理1)
layout (binding = 4) uniform sampler2D texD2; //纹理采样器(细节纹理2)
layout (binding = 5) uniform sampler2D texD3; //纹理采样器(细节纹理3)
layout (binding = 6) uniform sampler2D texD4; //纹理采样器(细节纹理4)
layout (location = 0) in vec2 inTexCoor; //接收的顶点纹理坐标
layout (location = 1) in vec4 inLightQD; //接收的最终光照强度
layout (location = 0) out vec4 outColor; //输出到管线的片元颜色
void main() { //主函数
    float dtScale1=27.36; //细节纹理1 的缩放系数
    float dtScale2=20.00; //细节纹理2 的缩放系数
    float dtScale3=32.34; //细节纹理3 的缩放系数
    float dtScale4=22.39; //细节纹理4 的缩放系数
    float ctSize=257; //地形灰度图的尺寸（以像素为单位）
    float factor1=ctSize/dtScale1; //细节纹理1 的纹理坐标缩放系数
    float factor2=ctSize/dtScale2; //细节纹理2 的纹理坐标缩放系数
    float factor3=ctSize/dtScale3; //细节纹理3 的纹理坐标缩放系数
    float factor4=ctSize/dtScale4; //细节纹理4 的纹理坐标缩放系数
    vec4 cT = textureLod(texC,inTexCoor,0.0); //从基础颜色纹理中采样
    vec4 dT = textureLod(texD,inTexCoor,0.0); //从过程纹理中采样
	vec4 dT1 = textureLod(texD1,inTexCoor*factor1,0.0); //从细节纹理1 中采样
	vec4 dT2 = textureLod(texD2,inTexCoor*factor2,0.0); //从细节纹理2 中采样
	vec4 dT3 = textureLod(texD3,inTexCoor*factor3,0.0); //从细节纹理3 中采样
	vec4 dT4 = textureLod(texD4,inTexCoor*factor4,0.0); //从细节纹理4 中采样
	outColor = dT1*dT.r+dT2*dT.g+dT3*dT.b+dT4*dT.a; //叠加细节纹理的颜色值
	outColor = outColor + cT; //叠加基础颜色值
	outColor = outColor - 0.5; //调整整体颜色
	outColor = inLightQD * outColor; //计算最终颜色值

}
