#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals {//一致变量块
    float brightFactor;//亮度调节系数
} myBufferVals;
layout (binding = 1) uniform sampler2D texOne;//第一幅纹理内容数据
layout (binding = 2) uniform sampler2D texTwo;//第二幅纹理内容数据
layout (binding = 3) uniform sampler2D texThree;//第三幅纹理内容数据
layout (binding = 4) uniform sampler2D texFour;//第四幅纹理内容数据
layout (binding = 5) uniform sampler2D texFive;//第五幅纹理内容数据
layout (location = 0) in vec2 inTexCoor;//接收从顶点着色器过来的纹理坐标数据
layout (location = 0) out vec4 outColor;//输出的片元颜色
void main() {
   vec4 finalColorOne= texture(texOne, inTexCoor);//对第一幅纹理进行采样
   vec4 finalColorTwo= texture(texTwo, inTexCoor);//对第二幅纹理进行采样
   vec4 finalColorThree= texture(texThree, inTexCoor);//对第三幅纹理进行采样
   vec4 finalColorFour= texture(texFour, inTexCoor);//对第四幅纹理进行采样
   vec4 finalColorFive= texture(texFive, inTexCoor);//对第五幅纹理进行采样
   outColor=myBufferVals.brightFactor*(0.6f*finalColorOne+0.1f*finalColorTwo+0.1f*finalColorThree+0.1f*finalColorFour+0.1f*finalColorFive);//根据权重不同计算产生最终的片元颜色值
}