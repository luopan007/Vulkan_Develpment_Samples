#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (push_constant) uniform constantVals {
	mat4 mvp;
	mat4 mm;
} myConstantVals;

layout (std140,set = 0, binding = 0) uniform bufferVals {
    vec4 uCamera;
} myBufferVals;

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inTexCoor;
layout (location = 0) out vec3 outTexCoor;
layout (location = 1) out float outDis;
out gl_PerVertex {
	vec4 gl_Position;
};

void main() {

    gl_Position = myConstantVals.mvp * vec4(inPos,1.0);
    outTexCoor = inTexCoor;

    vec4 pos = myConstantVals.mm*vec4(inPos,1.0);
	outDis = distance(myBufferVals.uCamera.xyz,pos.xyz);//顶点与摄像机距离
}
