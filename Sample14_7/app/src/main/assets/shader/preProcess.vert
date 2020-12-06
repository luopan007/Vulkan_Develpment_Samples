#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals { //推送常量块
mat4 mvp; //总变换矩阵
mat4 mm; //基本变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 2) in vec3 inNormal; //输入的法向量
layout (location = 0) out vec2 outTexCoor; //输出到片元着色器的纹理坐标
layout (location = 1) out vec3 outPos; //输出到片元着色器的世界坐标系中的位置坐标
layout (location = 2) out vec3 outNormal; //输出到片元着色器的世界坐标系中的法向量
out gl_PerVertex {vec4 gl_Position;}; //内建变量 gl_Position
vec3 normalFromObjectToWorld( //将物体坐标系的法向量变换到世界坐标系中的方法
in mat4 uMMatrix, //基本变换矩阵
in vec3 normal, //要变换的法向量
in vec3 position //顶点位置
){
vec3 normalTarget=position+normal; //计算变换后的法向量
vec3 newNormal=(uMMatrix*vec4(normalTarget,1)).xyz-(uMMatrix*vec4(position,1)).xyz;
newNormal=normalize(newNormal); //对法向量规格化
return newNormal; //返回计算变换后的法向量
}
void main() { //主方法
outTexCoor = inTexCoor; //将接收的纹理坐标传递给片元着色器
outNormal = normalFromObjectToWorld(myConstantVals.mm,inNormal,
pos); //将世界坐标系的法向量传给片元着色器
outPos =(myConstantVals.mm*vec4(pos,1)).xyz; //将世界坐标系中的位置传给片元着色器
gl_Position = myConstantVals.mvp * vec4(pos,1.0); //根据总变换矩阵计算此次绘制此顶点位置
}