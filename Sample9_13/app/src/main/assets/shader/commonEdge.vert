#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (push_constant) uniform constantVals{
    mat4 mvp;
} myConstantVals;
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inNormal;
out gl_PerVertex{
    vec4 gl_Position;
};
void main()
{
    vec3 position = pos;    // 获取此顶点的位置
    position.xyz += inNormal * 0.4; // 进行顶点挤出
    gl_Position = myConstantVals.mvp * vec4(position.xyz, 1);
}