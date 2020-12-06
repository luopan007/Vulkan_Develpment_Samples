#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致变量
    vec4 uCamera; //摄像机位置
} myBufferVals;
layout (push_constant) uniform constantVals { //推送常量
    mat4 mvp; //最终变换矩阵
    mat4 mm; //基本变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec3 inNormal; //输入的法向量
layout (location = 0) out vec3 outTexCoor; //输出到片元着色器的纹理坐标(立方图纹理)
out gl_PerVertex { //输出接口块
    vec4 gl_Position; //顶点最终位置
};
void main() {
    gl_Position = myConstantVals.mvp * vec4(pos,1.0); //计算顶点最终位置
    vec3 normalTarget=pos+inNormal; //计算世界坐标系中的法向量
    vec3 newNormal=(myConstantVals.mm*
    vec4(normalTarget,1)).xyz-(myConstantVals.mm*vec4(pos,1)).xyz;
    newNormal=normalize(newNormal); //对法向量规格化
    vec3 eye=-normalize //计算从观察点到摄像机的向量(视线向量)
    (myBufferVals.uCamera.xyz-(myConstantVals.mm*vec4(pos,1)).xyz);
    outTexCoor=reflect(eye,newNormal); //计算视线向量的反射向量并传递给片元着色器
}

