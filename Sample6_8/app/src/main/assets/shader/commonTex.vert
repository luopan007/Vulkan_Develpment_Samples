#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals {//推送常量块
	mat4 mvp; //最终变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos;//输入的顶点位置
out gl_PerVertex { //输出接口块
	vec4 gl_Position;//内建变量gl_Position
	float gl_PointSize; //内建变量gl_ PointSize
};
void main() {//主方法
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);//计算顶点最终位置
    gl_PointSize=32; //设置点精灵对应点的尺寸
}

