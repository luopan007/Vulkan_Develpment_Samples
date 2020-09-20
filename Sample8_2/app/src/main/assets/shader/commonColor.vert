#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (push_constant) uniform constantVals {
	mat4 mvp;
	float pointSize;
} myConstantVals;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 0) out vec3 vcolor;

out gl_PerVertex {
	vec4 gl_Position;
	float gl_PointSize;//用于点绘制方式时设置点的大小
};

void main() {
    gl_PointSize=myConstantVals.pointSize;
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);
    vcolor=color;
}
