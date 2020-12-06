#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals {//推送常量
	mat4 mvp;//最终变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos;//输入的顶点的位置
layout (location = 1) in vec2 inTexCoor;//输入的纹理坐标
layout (location = 0) out vec2 vTexPosition;//输出到片元着色器的纹理坐标
out gl_PerVertex { //输出接口块
	vec4 gl_Position;//顶点最终位置
};
void main() {
    vTexPosition = (inTexCoor-0.5)*5.0;//将纹理坐标转换后传递给片元着色器
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);//根据总变换矩阵计算此次绘制此顶点位置
}
