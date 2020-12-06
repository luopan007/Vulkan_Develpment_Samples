#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals {
	mat4 mvp;
} myConstantVals;
layout (std140,set = 0, binding =0 ) uniform bufferVals {//输入的一致块
  float uWidthSpan;//横向长度总跨度
  float uStartAngle;//起始角度
} myBufferVals;
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTexCoor;
layout (location = 0) out vec2 outTexCoor;
out gl_PerVertex {
	vec4 gl_Position;
};
void main() {
	//计算X向角度
   float angleSpanH=4.0*3.14159265;//横向角度总跨度，用于进行X距离与角度的换算
   float startX=-myBufferVals.uWidthSpan/2.0;//起始X坐标(即最左侧顶点的X坐标)
   //根据横向角度总跨度、横向长度总跨度及当前点X坐标折算出当前顶点X坐标对应的角度
   float currAngleH=myBufferVals.uStartAngle+((pos.x-startX)/myBufferVals.uWidthSpan)*angleSpanH;

   //计算出随Y向发展起始角度的扰动值
   float angleSpanZ=4.0*3.14159265;//纵向角度总跨度，用于进行Y距离与角度的换算
   float uHeightSpan=0.75*myBufferVals.uWidthSpan;//纵向长度总跨度
   float startY=-uHeightSpan/2.0;//起始Y坐标(即最上侧顶点的Y坐标)
   //根据纵向角度总跨度、纵向长度总跨度及当前点Y坐标折算出当前顶点Y坐标对应的角度
   float currAngleZ=((pos.y-startY)/uHeightSpan)*angleSpanZ;
   //计算斜向波浪
   float tzH=sin(currAngleH-currAngleZ)*2;//通过正弦函数求出当前点的Z坐标
    outTexCoor = inTexCoor;
    gl_Position = myConstantVals.mvp * vec4(pos.x,pos.y,tzH,1.0);
}
