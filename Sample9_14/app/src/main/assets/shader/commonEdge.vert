#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals{
     mat4 mvp;
} myConstantVals;
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inNormal;
out gl_PerVertex{
     vec4 gl_Position;
};
void main(){
vec3 position = pos; //获取此顶点的位置
vec4 ydskj = myConstantVals.mvp * vec4(0, 0, 0, 1); //将原点转化进视空间
vec4 fxldskj = myConstantVals.mvp * vec4(inNormal.xyz, 1.0); //将法向量转化进视空间
vec2 skjNormal = fxldskj.xy - ydskj.xy; //得到视空间中的法向量
skjNormal = normalize(skjNormal); //规格化法向量
vec4 finalPosition = myConstantVals.mvp * vec4(position.xyz, 1); //计算顶点最终位置
finalPosition = finalPosition / finalPosition.w; //执行透视除法
gl_Position = finalPosition + vec4(skjNormal.xy, 1.0, 1.0) * 0.01;//沿视空间中的法向量将顶点位置挤出
}