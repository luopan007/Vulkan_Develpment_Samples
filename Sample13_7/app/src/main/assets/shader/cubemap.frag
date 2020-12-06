#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex; //输入的纹理数据
layout (location = 0) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 0) out vec4 outColor; //输出到渲染管线的片元最终颜色值
layout (push_constant) uniform constantVals { //推送常量
layout(offset=64)vec4 color; //光晕元素的颜色值
} myConstantValsFrag;
void main() { //主方法
    vec4 finalColor=texture(tex, inTexCoor); //采样纹理颜色值
    outColor=finalColor*myConstantValsFrag.color; //计算最终颜色值并传递给渲染管线
}
