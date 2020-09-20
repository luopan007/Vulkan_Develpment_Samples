#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals {
	float brightFactor;
	float landStartY;
	float landYSpan;
} myBufferVals;
layout (binding = 1) uniform sampler2D texGrass;
layout (binding = 2) uniform sampler2D texRock;
layout (location = 0) in vec2 inTexCoor;
layout (location = 1) in float landHeight;
layout (location = 2) in float dis;
layout (location = 0) out vec4 outColor;

float mip_map_level(vec2 texCoor,vec2 texSize){
    vec2 texCoorActual=texCoor*texSize;			//计算片元的纹素坐标
    vec2 dx= dFdx(texCoorActual);					//计算纹素坐标在屏幕X方向的每片元变化量
    vec2 dy= dFdy(texCoorActual);					//计算纹素坐标在屏幕Y方向的每片元变化量
    float maxDeltaSquare = max(dot(dx,dx), dot(dy,dy));	//求出X、Y方向变化量点积的最大值
    float mml = log2(sqrt(maxDeltaSquare));			//求出对应的细节级别
    return max( 0, mml ); 						//返回细节级别值
}
void main() {

	float lodLevel=mip_map_level(inTexCoor,textureSize(texGrass,0));//求出细节级别值
	vec4 grassColor = textureLod(texGrass, inTexCoor, lodLevel);
	vec4 rockColor = textureLod(texRock, inTexCoor, lodLevel);
	vec4 finalColor;
	if(landHeight < myBufferVals.landStartY){
		finalColor = grassColor;
	}else if(landHeight > myBufferVals.landStartY + myBufferVals.landYSpan){
		finalColor = rockColor;
	}else{
		float currYRatio=(landHeight-myBufferVals.landStartY)/myBufferVals.landYSpan;
		finalColor = currYRatio * rockColor + (1.0 - currYRatio) * grassColor;
	}
	outColor=myBufferVals.brightFactor*finalColor;
}
