#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals { //推送常量
    mat4 mvp; //最终变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 2) in vec3 inNormal; //输入的法向量
layout (location = 3) in vec3 tNormal; //输入的切向量
layout (location = 0) out vec3 vPosition; //输出到片元着色器的顶点坐标
layout (location = 1) out vec2 vTextureCoord; //输出到片元着色器的纹理坐标
layout (location = 2) out vec3 fNormal; //输出到片元着色器的法向量
layout (location = 3) out vec3 ftNormal; //输出到片元着色器的切向量
out gl_PerVertex { //输出接口块
    vec4 gl_Position; //顶点最终位置
};
void main() {
    gl_Position = myConstantVals.mvp * vec4(pos,1.0); //计算顶点最终位置
    vPosition=pos; //将顶点坐标传递给片元着色器
    vTextureCoord = inTexCoor; //将纹理坐标传递给片元着色器
    fNormal=inNormal; //将法向量传递给片元着色器
    ftNormal=tNormal; //将切向量传递给片元着色器
}
