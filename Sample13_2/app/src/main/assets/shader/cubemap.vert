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

layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec3 inNormal; //输入的法向量
layout (location = 0) out vec3 newNormalVary; //用于传递给片元着色器的变换后法向量
layout (location = 1) out vec3 eyeVary; //用于传递给片元着色器的视线向量
out gl_PerVertex { //输出接口块
    vec4 gl_Position; //顶点最终位置
};
void main() {
	gl_Position = myConstantVals.mvp * vec4(pos,1.0); //根据总变换矩阵计算此次绘制此顶点的位置
	vec3 normalTarget=pos+inNormal; //计算世界坐标系中的法向量
	vec3 newNormal=(myConstantVals.mm*vec4(normalTarget,1)).xyz-(myConstantVals.mm*vec4(pos,1)).xyz;
	newNormalVary=normalize(newNormal); //将变换后的法向量传递给片元着色器
	eyeVary=-normalize(myBufferVals.uCamera.xyz-(myConstantVals.mm*vec4(pos,1)).xyz); //计算视线向量并传给片元着色器
}

