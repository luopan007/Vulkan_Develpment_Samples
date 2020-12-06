#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex;//纹理采样器，代表物体纹理
layout (binding = 2) uniform sampler2D texSnow; //纹理采样器，代表雪花纹理
layout (location = 0) in vec2 inTexCoor;//接收的顶点纹理坐标
layout (location = 1) in vec4 inLightQD; //接收的光照强度
layout (location = 2) in vec3 inNormal;//输出到管线的片元颜色
layout (location = 0) out vec4 outColor;//输出到管线的片元颜色
void main() {//主方法
   vec4 finalColorCommon=inLightQD*texture(tex, inTexCoor); 			//计算片元的非积雪颜色值
   vec4 finalColorSnow=inLightQD*texture(texSnow, inTexCoor);//计算片元的积雪颜色值
   float factor=dot(inNormal,vec3(0.0,1.0,0.0));//计算法向量与水平面法向量的点积
   factor=smoothstep(0.0,1.0,factor);//将积雪因子值进行平滑处理
   outColor=factor*finalColorSnow+(1-factor)*finalColorCommon;//根据因子值计算最终片元颜色值
}
