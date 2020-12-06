#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec3 inTexCoor;//接收从顶点着色器过来的3D 纹理坐标
layout (location = 1) in float inDis;//接收从顶点着色器过来的片元到摄像机的距离
layout (location = 0) out vec4 outColor;//输出到管线的片元颜色
void main() {//主方法
    outColor=vec4(inTexCoor,inDis);//最终输出到片元的颜色值为对应3D 纹理坐标
}