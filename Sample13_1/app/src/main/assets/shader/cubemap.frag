#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform samplerCube tex; //纹理内容数据（立方图）
layout (location = 0) in vec3 inTexCoor; //接收从顶点着色器传入的纹理坐标
layout (location = 0) out vec4 outColor; //输出到渲染管线的片元颜色值
void main() {
    outColor=textureLod(tex, inTexCoor, 0.0); //采样纹理颜色值
}
