#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的顶点纹理坐标
layout (location = 2) in vec3 inNormal; //输入的顶点法向量
layout (location = 0) out vec3 tcsNormal; //输出到细分控制着色器的顶点法向量
layout (location = 1) out vec2 tcsTexCoor; //输出到细分控制着色器的顶点纹理坐标
out gl_PerVertex { //输出接口块
vec4 gl_Position; //顶点位置
};
void main(){ //主函数
gl_Position = vec4(pos,1); //将顶点位置传递给渲染管线
tcsNormal=inNormal; //将法向量输出到细分控制着色器
tcsTexCoor=inTexCoor; //将纹理坐标输出到细分控制着色器
}
