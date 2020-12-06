#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (push_constant) uniform constantVals {
	mat4 mvp;
	mat4 mm;
} myConstantVals;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTexCoor;
layout (location = 2) in vec3 inNormal;
layout (location = 0) out vec4 vPosition;
out gl_PerVertex {
	vec4 gl_Position;
};
void main() {
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);
    vPosition=myConstantVals.mm*vec4(pos,1);//世界坐标系顶点传递给片元着色器
}
