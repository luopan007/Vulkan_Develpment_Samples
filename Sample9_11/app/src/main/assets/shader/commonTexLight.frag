#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex; //纹理数据（色块纹理）
layout (location = 0) in float vEdge; //接收到的描边系数
layout (location = 1) in vec2 inTexCoor; //接收到的纹理坐标
layout (location = 0) out vec4 outColor; //输出到管线的片元颜色
void main() {
    vec4 finalColor = textureLod(tex, inTexCoor, 0.0); //从纹理中采样出颜色
    const vec4 edgeColor = vec4(0.0); //描边的颜色（黑色）
    float mbFactor = step(0.2, vEdge); //根据描边系数确定描边因子
    outColor=(1.0 - mbFactor) * edgeColor + mbFactor * finalColor; //计算片元最终颜色值
}