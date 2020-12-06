#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex;//纹理采样器，代表一幅纹理
layout (location = 0) in vec2 inTexCoor;//接收的顶点纹理坐标
layout (location = 1) in vec4 inLightQD;//接收的光照强度
layout (location = 2) in vec4 fragCamera;//接收的摄像机位置
layout (location = 3) in vec4 vPosition;//接收的顶点位置
layout (location = 0) out vec4 outColor;//输出到管线的片元颜色
layout (location = 1) out float outDepth;//输出到管线的片元到摄像机的距离值
void main() {//主方法
    float dis=distance(vPosition.xyz,fragCamera.xyz);//计算当前片元到摄像机的距离
    outColor=inLightQD*texture(tex, inTexCoor);//计算片元的最终颜色值并输出
    outDepth=dis;//将当前片元到摄像机的距离输出
}