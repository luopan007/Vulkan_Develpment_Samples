#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals {//推送常量块

	mat4 mvp;//最终变换矩阵
	float arrayIndex;//2D 纹理数组索引
} myConstantVals;
layout (location = 0) in vec3 pos;//输入的顶点位置
layout (location = 1) in vec2 inTexCoor;//输入的顶点纹理坐标
layout (location = 0) out vec3 outTexCoor;//用于传递给片元着色器的纹理坐标
out gl_PerVertex {
	vec4 gl_Position;//内建变量 gl_Position
};
void main() {//主方法
    outTexCoor = vec3(inTexCoor,myConstantVals.arrayIndex);//组装纹理坐标
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);//计算顶点最终位置
}
