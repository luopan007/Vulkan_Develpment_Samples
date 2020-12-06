#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 0) uniform sampler2D tex; //纹理采样器，代表一幅纹理
layout (location = 0) in vec2 inTexCoor; //接收从顶点着色器过来的纹理坐标数据
layout (location = 1) in vec3 inPos; //接收从顶点着色器过来的顶点位置数据
layout (location = 2) in vec3 inNormal; //接收从顶点着色器过来的法向量数据
layout (location = 0) out vec4 outColor; //输出的片元颜色（无光照）
layout (location = 1) out vec4 outPos; //输出的顶点位置（世界坐标系）
layout (location = 2) out vec4 outNormal; //输出的法向量（世界坐标系）
void main() { //主方法
outColor=texture (tex, inTexCoor); //对纹理图进行采样，得到片元的颜色（无光照）
outPos=vec4(inPos.xyz,1.0); //输出世界坐标系中位置坐标的值
outNormal=vec4(inNormal.xyz,1.0); //输出世界坐标系中法向量的值
}