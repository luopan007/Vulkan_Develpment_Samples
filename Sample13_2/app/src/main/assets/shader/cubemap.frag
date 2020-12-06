#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform samplerCube tex; //纹理内容数据
layout (location = 0) in vec3 newNormalVary; //接收从顶点着色器过来的变换后法向量
layout (location = 1) in vec3 eyeVary; //接收从顶点着色器过来的视线向量
layout (location = 0) out vec4 outColor; //输出到渲染管线的片元颜色
void main() {
    const float zsl=0.93; //折射系数
    vec3 vTextureCoord=refract(eyeVary,newNormalVary,zsl);//根据斯涅尔定律计算折射后的视线向量
    outColor=texture(tex, vTextureCoord); //采样出最终纹理颜色
}
