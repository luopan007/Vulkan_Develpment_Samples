#version 400
#extension GL_ARB_separate_shader_objects : enable//打开GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable//打开GL_ARB_shading_language_420pack
layout (binding = 1) uniform sampler2D tex;//纹理采样器（代表一幅纹理）
layout (location = 0) in vec2 inTexCoor;//从顶点着色器接受到的纹理坐标
layout (location = 1) in vec4 inLightQD;//从纹理着色器接受到的光照强度
layout (location = 0) out vec4 outColor;//输出到渲染管线的最终颜色
void main() {
    vec4 finalColor = textureLod(tex, inTexCoor, 0.0);//从纹理中采样出颜色值
    finalColor.a=(finalColor.r+finalColor.g+finalColor.b)/3.0;//根据灰度值计算透明度
    outColor = inLightQD * finalColor;//将片元的最终颜色值传递给渲染管线
}

