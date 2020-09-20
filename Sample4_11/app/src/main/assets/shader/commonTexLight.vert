#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

////目前的实验其表明 必须是4的整数倍 否则奇怪现象
//layout (std140,set = 0, binding = 0) uniform bufferVals {
//    mat4 mvp;
//} myBufferVals;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 0) out vec3 vcolor;
layout (location = 1) out vec3 vPosition;

out gl_PerVertex {
	vec4 gl_Position;
};

layout (push_constant) uniform constantVals {
	mat4 mvp;
} myConstantVals;


void main() {
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);
    vPosition=gl_Position.xyz;
    vcolor=color;
}
