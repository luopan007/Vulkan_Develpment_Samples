#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals { //推送常量块
layout(offset = 0) mat4 mvp; //总变换矩阵
layout(offset = 64) vec4 needsNormal; //是否绘制法向量的控制标志
} myConstantVals;
layout (triangles) in; //输入图元为三角形
layout (line_strip) out; //输出图元为折线
layout (max_vertices = 9) out; //最大输出顶点数为9
layout (location = 0) in vec3 inTeNormal[3]; //从细分求值着色器传递过来的法向量(变换后)
layout (location = 1) in vec2 inVTexCoor[3]; //从细分求值着色器传递过来的纹理坐标
layout (location = 2) in vec3 inVPosition[3]; //从细分求值着色器传递过来的位置坐标
layout (location = 3) in vec3 teNormalOri[3]; //从细分求值着色器传递过来的法向量(原始的)
layout (location = 0) out vec3 teNormal; //输出到片元着色器的法向量
layout (location = 1) out vec2 vTexCoor; //输出到片元着色器的纹理坐标
layout (location = 2) out vec3 vPosition; //输出到片元着色器的位置坐标
layout (location = 3) out float fxlFlag; //输出到片元着色器的法向量绘制控制标志
in gl_PerVertex{ //输入接口块
vec4 gl_Position; //输入的顶点位置坐标
} gl_in[];
out gl_PerVertex { //输出接口块
vec4 gl_Position; //输出到片元着色器的顶点绘制位置
};
void main(void){
int i; //循环控制辅助变量
for (i = 0; i < 3; i++){ //遍历三角形的三个顶点
gl_Position = myConstantVals.mvp*gl_in[i].gl_Position; //将顶点绘制位置传递给管线
teNormal=inTeNormal[i]; //将法向量传递给片元着色器
vTexCoor=inVTexCoor[i]; //将纹理坐标传递给片元着色器
vPosition=inVPosition[i]; //将顶点位置传递给片元着色器
fxlFlag=0.0; //将法向量绘制控制标志传递给片元着色器
EmitVertex(); //结束一个顶点数据的输出
}
EndPrimitive(); //结束一个图元的输出
if(myConstantVals.needsNormal[0]>0.5){ //若需要绘制顶点的法向量
for (i = 0; i < 3; i++) { //遍历三角形的三个顶点
gl_Position = myConstantVals.mvp*gl_in[i].gl_Position; //计算顶点绘制位置(法向量线段起点)
teNormal=inTeNormal[i]; //将法向量传递给片元着色器
vTexCoor=inVTexCoor[i]; //将纹理坐标传递给片元着色器
vPosition=inVPosition[i]; //将顶点位置传递给片元着色器
fxlFlag=1.0; //将法向量绘制控制标志传递给片元着色器
EmitVertex(); //结束一个顶点数据的输出
vec3 fxlEnd=gl_in[i].gl_Position.xyz+teNormalOri[i]*2.0; //法向量线段终点位置
gl_Position = myConstantVals.mvp*(vec4(fxlEnd,1)); //计算顶点绘制位置(法向量线段终点)
teNormal=inTeNormal[i]; //将法向量传递给片元着色器
vTexCoor=inVTexCoor[i]; //将纹理坐标传递给片元着色器
vPosition=fxlEnd; //将顶点位置传递给片元着色器
fxlFlag=1.0; //将法向量绘制控制标志传递给片元着色器
EmitVertex(); //结束一个顶点数据的输出
EndPrimitive(); //结束一个图元的输出
}}}