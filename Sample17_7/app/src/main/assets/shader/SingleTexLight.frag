#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform samplerCube tex;
layout (location = 0) in vec3 newNormalVary;
layout (location = 1) in vec3 eyeVary;
layout (location = 2) in vec4 inLightQD;
layout (location = 0) out vec4 outColor;
vec4 zfs( //计算折反射纹理采样颜色的方法
in float zsl){ //折射率
	vec3 vTextureCoord; //用于进行立方图纹理采样的向量
	vec4 finalColorZS; //若是折射的采样结果
	vec4 finalColorFS; //若是反射的采样结果
	vec4 finalColor; //最终颜色
	const float maxH=0.9; //入射角余弦值若大于此值则仅计算折射
	const float minH=0.8; //入射角余弦值若小于此值则仅计算反射
	float sizeH=maxH-minH; //混合时入射角余弦值的跨度
	float testValue=abs(dot(eyeVary,newNormalVary)); //计算视线向量与法向量夹角的余弦值
	if(testValue>maxH) { //余弦值大于maxH 仅折射
		vTextureCoord=refract(eyeVary,newNormalVary,zsl); //计算纹理坐标(折射)
		finalColor=texture(tex, vTextureCoord); //采样纹理颜色
	} else if(testValue<=maxH&&testValue>=minH) { //余弦值在minH～maxH 范围内反射、折射融合
		vTextureCoord=reflect(eyeVary,newNormalVary); //计算纹理坐标(反射)
		finalColorFS=texture(tex, vTextureCoord); //反射的计算结果
		vTextureCoord=refract(eyeVary,newNormalVary,zsl); //计算纹理坐标(折射)
		finalColorZS=texture(tex, vTextureCoord); //折射的计算结果
		float ratio=(testValue-minH)/sizeH; //融合比例因子
		finalColor=finalColorZS*ratio+(1.0-ratio)*finalColorFS; //折反射结果线性融合
	} else{ //余弦值小于minH 仅反射
		vTextureCoord=reflect(eyeVary,newNormalVary); //计算纹理坐标(反射)
		finalColor=texture(tex, vTextureCoord); //反射的计算结果
	}
	return finalColor; //返回最终颜色值
}
void main() {
   vec4 finalColor=vec4(0.0,0.0,0.0,0.0); //给定初始化颜色值
   	finalColor.r=zfs(0.9).r; //计算红色通道的采样结果
   	finalColor.g=zfs(0.9).g; //计算绿色通道的采样结果
   	finalColor.b=zfs(0.9).b; //计算蓝色通道的采样结果
   	outColor=finalColor; //将最终的片元颜色传递给管线
}