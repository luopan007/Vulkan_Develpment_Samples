#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致变量块
    float brightFactor; //亮度调节系数
} myBufferVals;
layout (binding = 1) uniform sampler2D tex; //纹理采样器，代表一幅纹理
layout (location = 0) in vec2 inTexCoor; //接收从顶点着色器过来的纹理坐标数据
layout (location = 0) out vec4 outColor; //输出的片元颜色
void main() {
    vec4 finalColor=texture(tex, inTexCoor); //对纹理图进行采样
    outColor = finalColor; //最终片元的颜色
}
