#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals{ //推送常量块
mat4 mvp; //最终变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec3 inNormal; //输入的法向量
out gl_PerVertex{ //输出接口块
vec4 gl_Position; //内建变量gl_Position
};
void main(){
    vec3 position = pos; //获取此顶点的位置
    position.xyz += inNormal * 0.4; //沿法向量方向进行顶点挤出
    gl_Position = myConstantVals.mvp * vec4(position.xyz, 1); //计算顶点的最终位置
 }