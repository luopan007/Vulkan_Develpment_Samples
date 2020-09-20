#version 400
#extension GL_ARB_separate_shader_objects : enable //打开 GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable //打开 GL_ARB_shading_language_420pack
layout (location = 0) out vec4 outColor; //输出到渲染管线的最终片元颜色值
layout (location = 0) in vec4 inLightQD; //顶点着色器传入的光照强度
layout (location = 1) in float vFogFactor; //顶点着色器传入的雾化因子
void main(){
    vec4 objectColor=vec4(0.95,0.95,0.95,1.0); //物体颜色
    vec4 fogColor = vec4(0.97,0.76,0.03,1.0); //雾的颜色
if(vFogFactor != 0.0) { //若雾化因子不为 0
    objectColor = objectColor*inLightQD; //计算光照后物体的颜色
    outColor = objectColor*vFogFactor + fogColor*(1.0-vFogFactor); //计算最终的输出颜色
}else{ //若雾化因子为 0
    outColor=fogColor; //直接使用雾颜色作为最终颜色
}
}
