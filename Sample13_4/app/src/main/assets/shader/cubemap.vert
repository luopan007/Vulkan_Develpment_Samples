#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals {
    vec4 uCamera;
} myBufferVals;
layout (push_constant) uniform constantVals {
	mat4 mvp;
	mat4 mm;
} myConstantVals;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inNormal;
layout (location = 0) out vec3 newNormalVary;
layout (location = 1) out vec3 eyeVary;
out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);
	vec3 normalTarget=pos+inNormal;
    vec3 newNormal=(myConstantVals.mm*vec4(normalTarget,1)).xyz-(myConstantVals.mm*vec4(pos,1)).xyz;
    newNormalVary=normalize(newNormal);
    eyeVary=-normalize(myBufferVals.uCamera.xyz-(myConstantVals.mm*vec4(pos,1)).xyz);
}
