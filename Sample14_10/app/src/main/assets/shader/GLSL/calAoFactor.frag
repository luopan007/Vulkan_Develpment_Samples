#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 0) uniform sampler2D positionUni;
layout (binding = 2) uniform sampler2D fragDisUni;
layout (std140,set = 0, binding = 1) uniform bufferVals {
    vec4 uCamera;						
	mat4 mZHMatrix;
} myBufferVals;
layout (location = 0) in vec2 inTexCoor;
layout (location = 0) out vec4 outAoFactor;
const int PCount=16;
const vec3[PCount] points=
{
vec3(-1.4109830789736655,0.41551981760050594,1.3551642085117352), vec3(-1.71563098357675,0.12153334149424075,-1.0207056260728247), 
vec3(-1.5101061431210312,1.0916808145093917,0.7265070101108699), vec3(1.9069579480132612,-0.21607449824086555,0.5628704964012681), 
vec3(-1.0342702341865935,0.8133429356961823,1.5062398054854098), vec3(-1.3095804004461502,0.26983446269612166,-1.487342777408354), 
vec3(1.236515088080815,-1.477610154558723,-0.5363754916972261), vec3(1.4635171065477042,-1.23998466338527,0.5661763977874343), 
vec3(0.8394943992168215,0.10181501366699974,1.8124245795827139), vec3(1.5726076476390185,-0.4356637412915275,-1.1563313932913615), 
vec3(0.6378995218859248,-1.729368315533456,-0.7761246222139109), vec3(0.9945307789956596,-1.3632506714580492,1.0735250981693278), 
vec3(-0.19426588792219437,0.33826635898908297,1.961590333164922), vec3(-0.5288288092531159,-1.8756918542426992,0.449577755712532), 
vec3(0.3471964715180122,1.4894895198787435,-1.2887496189473082), vec3(-1.9821472826745812,0.03471594417648212,0.2643614060442807)
};
vec2 fromWorldToST(vec3 posIn,mat4 VP)
{
	vec4 jckjPosition=VP*vec4(posIn.xyz,1.0); 
	jckjPosition=jckjPosition/jckjPosition.w;
	float s=(jckjPosition.x+1.0)/2.0;		
	float t=(jckjPosition.y+1.0)/2.0;		
	return vec2(s,t);
}
void main()
{
	vec3 postion = textureLod(positionUni, inTexCoor, 0.0).rgb;
	float AOCount=0.0;
	for(int i=0;i<PCount;i++)
	{
		vec3 posTemp=postion+points[i];
		float LTemp=distance(myBufferVals.uCamera.xyz,posTemp);	
		vec2 stTemp=fromWorldToST(posTemp,myBufferVals.mZHMatrix);
		vec3 posTempL=textureLod(positionUni, stTemp, 0.0).rgb;
		float LTempL=textureLod(fragDisUni, stTemp, 0.0).r;
		if(LTempL<LTemp)
		{
			AOCount=AOCount+1.0;
		}
	}
	float aoFactor=1.0-AOCount/float(PCount);
	aoFactor=pow(aoFactor, 2.0);
	outAoFactor=vec4(aoFactor,0.0,0.0,1.0);
}