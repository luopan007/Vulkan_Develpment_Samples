#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex;//纹理采样器，代表一幅纹理
layout (location = 0) in vec2 inTexCoor;//接收从顶点着色器过来的纹理坐标数据
layout (location = 1) in vec4 inLightQD;//接收从顶点着色器过来的光照强度数据
layout (location = 0) out vec4 outColor;//输出的片元颜色（多重渲染目标0）
layout (location = 1) out float outDepth;//输出的片元深度（多重渲染目标1）
void main() //主方法
{
   vec4 finalColor=inLightQD*texture(tex, inTexCoor, 0.0);//根据光照强度和纹理采样颜色得到最终颜色
   outColor=finalColor;//输出此片元的颜色
   outDepth=gl_FragCoord.z;//输出此片元的深度
}