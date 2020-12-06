#version 400
#extension GL_ARB_separate_shader_objects:enable //开启GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack:enable //开启GL_ARB_shading_language_420pack
layout (binding = 1) uniform sampler2D tex; //纹理采样器，代表一幅纹理
layout (location = 0) in vec2 inTexCoor; //顶点着色器传入的顶点纹理坐标
layout (location = 1) in vec4 inLightQD; //顶点着色器传入的最终光照强度
layout (location = 0) out vec4 outColor; //输出到管线的片元最终颜色
void main() { //主方法
float ss = fract(inTexCoor.s * 20); //缩放片元纹理坐标S 分量并提取结果的小数部分
float tt = fract(inTexCoor.t * 20); //缩放片元纹理坐标T 分量并提取结果的小数部分
if ((ss > 0.2) && (tt > 0.2))discard; //上述提取的两个值超过阈值则丢弃该片元
outColor=inLightQD*textureLod(tex, inTexCoor, 0.0); //计算最终片元颜色值
}
