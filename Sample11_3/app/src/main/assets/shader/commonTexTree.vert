#version 400
#extension GL_ARB_separate_shader_objects : enable //开启separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable //开启shading_language_420pack
layout (push_constant) uniform constantVals { //推送常量块
    mat4 mvp; //总变换矩阵
} myConstantVals;
layout (std140,set = 0, binding =0 ) uniform bufferVals { //一致块
    float bend_R; //弯曲半径
    float direction_degree; //风的方向
} myBufferVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 0) out vec2 outTexCoor; //输出到片元着色器的纹理坐标
out gl_PerVertex { //输出接口块
vec4 gl_Position; //内建变量gl_Position
};
void main() {
    float current_radian=pos.y/myBufferVals.bend_R; //计算当前的弧度(θ)
    float pianDistance=myBufferVals.bend_R-myBufferVals.bend_R*cos(current_radian);//偏移距离
    float result_X=pos.x+pianDistance*sin(radians(myBufferVals.direction_degree));//偏移后的X 坐标
    float result_Y=myBufferVals.bend_R*sin(current_radian); //偏移后的Y 坐标
    float result_Z=pos.z+pianDistance*cos(radians(myBufferVals.direction_degree));//偏移后的Z 坐标
    vec4 tPostion=vec4(result_X,result_Y,result_Z,1.0); //得到最后的坐标
    outTexCoor = inTexCoor; //传输到片元着色器的纹理坐标
    gl_Position = myConstantVals.mvp * tPostion; //计算顶点最终位置
}
