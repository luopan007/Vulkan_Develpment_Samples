#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec4 inLightQD; //顶点着色器传入的最终光照强度
layout (location = 1) in vec2 mcLongLat; //顶点着色器传入的顶点经纬度
layout (location = 0) out vec4 outColor; //输出到渲染管线的片元颜色值
void main() { //主方法
	vec3 bColor=vec3(0.678,0.231,0.129); //砖块的颜色
	vec3 mColor=vec3(0.763,0.657,0.614); //水泥的颜色
	vec3 color; //片元的最终颜色
	int row=int(mod((mcLongLat.y+90.0)/12.0,2.0)); //计算当前片元位于奇数行还是偶数行
	float ny=mod(mcLongLat.y+90.0,12.0); //计算当前片元是否在此行区域1 中的辅助变量
	float oeoffset=0.0; //每行的砖块偏移值(奇数行偏移半个砖块)
	float nx; //计算当前片元是否在此行区域3 中的辅助变量
	if(ny>10.0){ color=mColor; } //若位于此行的区域1 中，采用水泥色着色
	else{ //若不位于此行的区域1 中
	if(row==1){ oeoffset=11.0; } //若为奇数行则偏移半个砖块
	nx=mod(mcLongLat.x+oeoffset,22.0); //计算当前片元是否在此行区域3 中的辅助变量
	if(nx>20.0){ color=mColor; } //若不位于此行的区域3 中，采用水泥色着色
	else{ color=bColor; } //若位于此行的区域3 中，采用砖块色着色
	}
	outColor=vec4(color,0)*inLightQD; //将片元的最终颜色传递进渲染管线
}
