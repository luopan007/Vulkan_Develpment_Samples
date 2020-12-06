#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding =0 ) uniform bufferVals {//一致变量块
  float uT;//混合比例因子
} myBufferVals;
layout (binding = 1) uniform sampler2D tex1;//纹理采样器，代表一幅纹理(照片1)
layout (binding = 2) uniform sampler2D tex2;//纹理采样器，代表一幅纹理(照片2)
layout (location = 0) in vec2 inTexCoor;//顶点着色器传入的纹理坐标
layout (location = 0) out vec4 outColor;//输出到渲染管线的片元颜色值
void main() {
    vec4 color1=textureLod(tex1, inTexCoor, 0.0);//采样纹理(照片1)
    vec4 color2=textureLod(tex2, inTexCoor, 0.0);//采样纹理(照片2)
   outColor=mix(color1,color2,myBufferVals.uT);//计算最终片元颜色值
}