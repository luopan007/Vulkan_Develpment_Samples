#version 400
#extension GL_ARB_separate_shader_objects : enable //开启separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable //开启shading_language_420pack
layout (push_constant) uniform constantVals { //推送常量块
    mat4 mvp; //总变换矩阵
} myConstantVals;
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致块
    float uBfb; //融合比例
} myBufferVals;
layout (location = 0) in vec3 aPosition; //顶点位置(来自1 号关键帧)
layout (location = 1) in vec3 bPosition; //顶点位置(来自2 号关键帧)
layout (location = 2) in vec3 cPosition; //顶点位置(来自3 号关键帧)
layout (location = 3) in vec2 inTexCoor; //顶点纹理坐标
layout (location = 0) out vec2 outTexCoor; //输出到片元着色器的纹理坐标
out gl_PerVertex { //输出接口块
    vec4 gl_Position; //内建变量gl_Position
};
void main() {
    vec3 tPosition; //融合后的结果顶点坐标
    if(myBufferVals.uBfb<=1.0){ //若融合比例小于等于1，则需要执行的是1、2 号关键帧的融合
    tPosition=mix(aPosition,bPosition,myBufferVals.uBfb);
    }else{ //若融合比例大于1，则需要执行的是2、3 号关键帧的融合
    tPosition=mix(bPosition,cPosition,myBufferVals.uBfb-1.0);
    }
    outTexCoor = inTexCoor; //输出到片元着色器的纹理坐标
    gl_Position = myConstantVals.mvp * vec4(tPosition,1.0); //计算最终的顶点位置
}
