#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform samplerCube tex; //纹理数据内容
layout (location = 0) in vec3 newNormalVary; //接收从顶点着色器过来的变换后法向量
layout (location = 1) in vec3 eyeVary; //接收从顶点着色器过来的视线向量
layout (location = 0) out vec4 outColor; //输出到渲染管线的最终片元颜色值
vec4 zs( //根据法向量、视线向量及斯涅尔定律计算立方图纹理采样的方法
in float zsl){ //折射率
	vec3 vTextureCoord=refract(eyeVary,newNormalVary,zsl); //根据斯涅尔定律计算折射后的视线向量
	vec4 finalColor=texture(tex, vTextureCoord); //进行立方图纹理采样
	return finalColor; //返回采样颜色值
}
void main() {
	vec4 finalColor=vec4(0.0,0.0,0.0,0.0); //给定初始化颜色初始值
	finalColor.r=zs(0.97).r; //计算红色通道的采样结果
	finalColor.g=zs(0.955).g; //计算绿色通道的采样结果
	finalColor.b=zs(0.94).b; //计算蓝色通道的采样结果
	outColor=finalColor; //将片元最终颜色值输出到渲染管线
}
