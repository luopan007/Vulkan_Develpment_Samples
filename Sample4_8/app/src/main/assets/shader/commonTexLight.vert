#version 400
#extension GL_ARB_separate_shader_objects : enable//启动GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable//启动GL_ARB_shading_language_420pack
layout (push_constant) uniform constantVals {			 //推送常量块
	mat4 mvp;								 //最终变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos;					//传入的物体坐标系顶点坐标
layout (location = 1) in vec3 color;					//传入的顶点颜色
layout (location = 0) out vec3 vcolor;					//传到片元着色器的顶点颜色
out gl_PerVertex { vec4 gl_Position;};				//输出接口块
void main() {									//主函数
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);	//计算顶点最终位置
    vcolor=color;								//传递顶点颜色给片元着色器
}
