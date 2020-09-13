#version 400
#extension GL_ARB_separate_shader_objects : enable//启动GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable//启动GL_ARB_shading_language_420pack
layout (std140, set = 0, binding = 0) uniform bufferVals { //一致块
    mat4 mvp;//总变换矩阵
} myBufferVals;
layout (location = 0) in vec3 pos;//传入的物体坐标系顶点位置
layout (location = 1) in vec3 color;//传入的顶点颜色
layout (location = 0) out vec3 vcolor;//传到片元着色器的顶点颜色
out gl_PerVertex { //输出的接口块
    vec4 gl_Position;//顶点最终位置
};
void main() { //主函数
    gl_Position = myBufferVals.mvp * vec4(pos, 1.0);//计算最终顶点位置
    vcolor=color;//传递顶点颜色给片元着色器
}
