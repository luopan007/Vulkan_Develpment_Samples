#version 400
#extension GL_ARB_separate_shader_objects : enable //开启separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable //开启shading_language_420pack
layout (push_constant) uniform constantVals { //推送常量块
mat4 mvp; //总变换矩阵
} myConstantVals;
layout (std140,set = 0, binding =0 ) uniform bufferVals { //一致块
float angleSpan; //本帧扭曲总角度
float yStart; //Y 坐标起始点
float ySpan; //Y 坐标总跨度
} myBufferVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 0) out vec2 outTexCoor; //输出到片元着色器的纹理坐标
out gl_PerVertex { //输出接口块
vec4 gl_Position; //内建变量gl_Position
};
void main() {
    float currAngle= myBufferVals.angleSpan*( //计算当前顶点扭动(即绕中心点旋
    pos.y-myBufferVals.yStart)/myBufferVals.ySpan; //转)的角度
    vec3 tPosition=pos; //拷贝当前顶点的坐标
    if(pos.y>myBufferVals.yStart){ //如果不是最下面一层
    tPosition.x=(cos(currAngle)*pos.x-sin(currAngle)*pos.z); //计算当前顶点扭动后的X 坐标
    tPosition.z=(sin(currAngle)*pos.x+cos(currAngle)*pos.z); //计算当前顶点扭动后的Z 坐标
     }
    outTexCoor = inTexCoor; //输出到片元着色器的纹理坐标
    gl_Position = myConstantVals.mvp * vec4(tPosition,1.0); //计算顶点最终位置
}
