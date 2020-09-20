#version 400
#extension GL_ARB_separate_shader_objects:enable //开启GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack:enable //开启GL_ARB_shading_language_420pack
layout (location = 0) out vec4 outColor; //输出到渲染管线的最终片元颜色值
layout (location = 0) in vec4 inLightQD; //顶点着色器传入的正面最终光照强度
layout (location = 1) in vec4 inLightQDBack; //顶点着色器传入的反面最终光照强度
void main() { //主函数
vec4 finalColor=vec4(0.9,0.9,0.9,1.0); //片元的原始颜色值
if(gl_FrontFacing) { //若当前片元是正面
outColor=inLightQD*finalColor; //给此片元正面颜色值
} else{ //若当前片元是反面
outColor=inLightQDBack*finalColor; //给此片元反面颜色值
}}
