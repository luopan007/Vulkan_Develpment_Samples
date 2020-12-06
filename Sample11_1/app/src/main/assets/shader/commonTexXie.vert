#version 400
#extension GL_ARB_separate_shader_objects : enable //开启separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable //开启shading_language_420pack
layout (push_constant) uniform constantVals { //常量块
mat4 mvp; //总变换矩阵
} myConstantVals;
layout (std140,set = 0, binding =0 ) uniform bufferVals { //一致块
float uWidthSpan; //横向长度总跨度
float uStartAngle; //起始角度
} myBufferVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 0) out vec2 outTexCoor; //输出到片元着色器的纹理坐标
out gl_PerVertex { //输出接口块
vec4 gl_Position; //内建变量gl_Position
};
void main() {
	float angleSpanH=4.0*3.14159265; //横向角度总跨度，用于进行X 距离与角度的换算
    float startX=-myBufferVals.uWidthSpan/2.0; //起始X 坐标(即最左侧顶点的X 坐标)
    float currAngleH=myBufferVals.uStartAngle+((pos.x-startX)/myBufferVals.uWidthSpan)*angleSpanH;
    float angleSpanZ=4.0*3.14159265; //纵向角度总跨度，用于进行Y 距离与角度的换算
    float uHeightSpan=0.75*myBufferVals.uWidthSpan; //纵向长度总跨度
    float startY=-uHeightSpan/2.0; //起始Y 坐标(即最上侧顶点的Y 坐标)
    float currAngleZ=((pos.y-startY)/uHeightSpan)*angleSpanZ; //计算当前Y 坐标对应的角度
    float tzH=sin(currAngleH-currAngleZ)*0.3; //通过正弦函数求出当前点的Z 坐标
    outTexCoor = inTexCoor; //输出到片元着色器的纹理坐标
    gl_Position = myConstantVals.mvp * vec4(pos.x,pos.y,tzH,1.0); //计算顶点最终位置
}
