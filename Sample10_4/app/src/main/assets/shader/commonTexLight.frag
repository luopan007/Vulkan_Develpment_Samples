#version 400
#extension GL_ARB_separate_shader_objects:enable //开启GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack:enable //开启GL_ARB_shading_language_420pack
layout (location = 0) out vec4 outColor; //输出到渲染管线的最终片元颜色
layout (location = 0) in vec4 inLightQD; //顶点着色器传入的光照强度
layout (location = 1) in float u_clipDist; //顶点着色器传入的剪裁数据
void main() { //主方法
if(u_clipDist < 0.0) discard; //若片元在剪裁平面的下侧，则丢弃该片元
outColor=inLightQD*vec4(0.9,0.9,0.9,1.0); //计算最终片元颜色
}
