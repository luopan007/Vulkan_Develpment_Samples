#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals { //推送常量块
mat4 mvp; //最终变换矩阵
mat4 mm; //基本变换矩阵
vec4 uCamera; //摄像机位置
} myConstantVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 0) out vec2 outTexCoor; //输出到片元着色器的纹理坐标
layout (location = 1) out float landHeight; //输出到片元着色器的Y 坐标
layout (location = 2) out float dis; //世界坐标系中顶点坐标到摄像机的距离
out gl_PerVertex { //输出接口块
vec4 gl_Position; //内建变量gl_Position
};
void main() {
outTexCoor = inTexCoor; //将接收的纹理坐标传递到片元着色器
landHeight = pos.y; //将顶点的Y 坐标传递给片元着色器
gl_Position = myConstantVals.mvp * vec4(pos,1.0); //计算顶点的最终位置
vec3 posWorld = (myConstantVals.mm * vec4(pos, 1.0)).xyz; //获取世界坐标系中顶点的坐标
dis = distance(posWorld, myConstantVals.uCamera.xyz); //计算出顶点位置到摄像机的距离
}