#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex; //纹理采样器，代表一幅纹理
layout (location = 0) in vec2 inTexCoor; //从顶点着色器传入的纹理坐标
layout (location = 1) in vec4 inLightQD; //从顶点着色器传入的光照强度
layout (location = 0) out vec4 outColor; //对应 0 号颜色附件的输出变量
layout (location = 1) out vec4 outColorR; //对应 1 号颜色附件的输出变量
layout (location = 2) out vec4 outColorG; //对应 2 号颜色附件的输出变量
layout (location = 3) out vec4 outColorB; //对应 3 号颜色附件的输出变量
void main() { //主方法
	vec4 finalColor=inLightQD*texture(tex, inTexCoor); //计算最终颜色值
	outColor=finalColor; //将 RGBA 四个色彩通道综合值输出到 0 号颜色附件
	outColorR=vec4(finalColor.r,0.0,0.0,1.0); //将 R 色彩通道值输出到 1 号颜色附件
	outColorG=vec4(0.0,finalColor.g,0.0,1.0); //将 G 色彩通道值输出到 2 号颜色附件
	outColorB=vec4(0.0,0.0,finalColor.b,1.0); //将 B 色彩通道值输出到 3 号颜色附件
}
