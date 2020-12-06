#version 450
#extension GL_ARB_separate_shader_objects : enable //开启separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable //开启shading_language_420pack
layout (push_constant) uniform constantVals { //推送常量块
    mat4 mvp; //总变换矩阵
    mat4 mm;//基本变换矩阵
} myConstantVals;
layout (std140,set = 0, binding =0 ) uniform bufferVals { //一致块
    float maxLifeSpan;//最大生命周期
    float bj;//粒子半径
    vec4 uCamera; //摄像机位置
} myBufferVals;
layout (location = 0) in vec4 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 0) out float sjFactor; //输出的衰减因子
layout (location = 1) out vec4 vPosition; //输出到片元着色器的顶点位置
out gl_PerVertex { //输出接口块
    vec4 gl_Position; //内建变量gl_Position
    float gl_PointSize;//内建变量gl_PointSize
};
void main() {
    //求出变换后顶点在世界坐标系中的位置
    vec4 currPosition=myConstantVals.mm * vec4(pos.xy,0.0,1);
    //求出顶点到摄像机的距离
    float d=distance(currPosition.xyz,myBufferVals.uCamera.xyz);
    //求出距离缩放因子S的平方分之1
    float s=1.0/sqrt(0.01+0.05*d+0.001*d*d);
    gl_PointSize=myBufferVals.bj*s;//设置点精灵对应点的尺寸
    gl_Position = myConstantVals.mvp* vec4(pos.x,pos.y,0.0,1.0);
    vPosition=vec4(pos.x,pos.y,0.0,pos.w);//计算顶点位置属性，并将其传递给片元着色器
    sjFactor=(myBufferVals.maxLifeSpan-pos.w)/myBufferVals.maxLifeSpan;//计算总衰减因子，并将其传递给片元着色器
}
