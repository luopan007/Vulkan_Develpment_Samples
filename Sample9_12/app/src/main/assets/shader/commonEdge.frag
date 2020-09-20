#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) out vec4 outColor; //输出到管线的片元颜色
void main(){
outColor = vec4(0.0, 1.0, 0.0, 0.0); //描边颜色(这里为绿色)
}