#version 400
#extension GL_ARB_separate_shader_objects : enable//开启separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable//开启shading_language_420pack
layout (std140,set = 0, binding = 0) uniform bufferVals {//一致块
	vec4 lightAmbient;									//环境光强度
} myBufferVals;
layout (push_constant) uniform constantVals{//推送常量块
	mat4 mvp;//最终变换矩阵
	mat4 mm;//基本变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos;//输入的顶点位置
layout (location = 0) out vec4 outLightQD;//输出的光照强度
layout (location = 1) out vec3 vposition;//输出的顶点位置
out gl_PerVertex { //输出接口块
	vec4 gl_Position;//内建变量gl_Position
};
void main(){//主函数
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);//计算顶点最终位置
    outLightQD=myBufferVals.lightAmbient;//将顶点最终光照强度传递给片元着色器
    vposition=pos;//将顶点位置传给片元着色器
}
