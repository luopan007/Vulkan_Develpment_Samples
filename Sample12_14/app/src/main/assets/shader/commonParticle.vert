#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals { //推送常量
    mat4 mvp; //总变换矩阵
} myConstantVals;
layout (std140,set = 0, binding =0 ) uniform bufferVals { //一致变量
    float maxLifeSpan; //最大生命周期
} myBufferVals;
layout (location = 0) in vec2 pos; //输入的顶点XY 坐标
layout (location = 1) in float xSpeed; //当前粒子x 方向速度
layout (location = 2) in float curLifespan; //当前粒子生命周期
layout (location = 3) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 0) out vec2 outTexCoor; //输出到片元着色器的纹理坐标
layout (location = 1) out float sjFactor; //输出的当前粒子的总衰减因子
layout (location = 2) out vec4 vPosition; //输出到片元着色器的顶点各项数据
out gl_PerVertex { //输出接口块
    vec4 gl_Position; //内建变量gl_Position
};
void main() {
    outTexCoor = inTexCoor; //传递给片元着色器的纹理坐标
    gl_Position = myConstantVals.mvp* vec4(pos.x,pos.y,0.0,1.0); //计算最终的顶点位置
    vPosition=vec4(pos.x,pos.y,0.0,curLifespan); //将顶点各项数据传递给片元着色器
    sjFactor=(myBufferVals.maxLifeSpan-curLifespan)/myBufferVals.maxLifeSpan;//总衰减因子
}
