#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
out gl_PerVertex{vec4 gl_Position;}; //内建变量gl_Position
const vec2 xyArr[3] = vec2[3]( //构成覆盖屏幕的三角形的三个点的坐标数组
    vec2(-1.0,-1.0), //第1 个点的X、Y 坐标
    vec2(3.0,-1.0), //第2 个点的X、Y 坐标
    vec2(-1.0,3.0) //第3 个点的X、Y 坐标
);
void main() {
    gl_Position = vec4(xyArr[gl_VertexIndex], 0.0f, 1.0f);//给出顶点的最终绘制位置
}