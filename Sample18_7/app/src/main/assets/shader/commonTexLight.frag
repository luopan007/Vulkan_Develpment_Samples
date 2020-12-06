#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals {
    vec4 uCamera;
	vec4 lightPosition;
	vec4 lightAmbient;
	vec4 lightDiffuse;
	vec4 lightSpecular;
} myBufferVals;
layout (binding = 1) uniform sampler2D tex;
layout (location = 0) in vec3 teNormal;
layout (location = 1) in vec2 vTexCoor;
layout (location = 2) in vec3 vPosition;
layout (location = 3) in float fxlFlag;
layout (location = 0) out vec4 outColor;
vec4 pointLight(				
  in vec3 inNormal,				
  in vec3 inCamera,				
  in vec3 lightLocation,		
  in vec3 currPosition,			
  in vec4 lightAmbient,			
  in vec4 lightDiffuse,			
  in vec4 lightSpecular			
){
  vec4 ambient=lightAmbient;	
  vec3 eye= normalize(inCamera-currPosition);
  vec3 vp= normalize(lightLocation-currPosition);
  vp=normalize(vp);
  vec3 halfVector=normalize(vp+eye);	
  float shininess=50.0;				
  float nDotViewPosition=max(0.0,dot(inNormal,vp)); 	
  vec4 diffuse=lightDiffuse*nDotViewPosition;			
  float nDotViewHalfVector=dot(inNormal,halfVector);	
  float powerFactor=max(0.0,pow(nDotViewHalfVector,shininess)); 	
  vec4 specular=lightSpecular*powerFactor;    			
  return ambient+diffuse+specular;						
}
void main() {
	vec4 gzqd=pointLight
	(
  		teNormal,			
  		myBufferVals.uCamera.xyz,			
  		myBufferVals.lightPosition.xyz,		
  		vPosition,			
  		myBufferVals.lightAmbient,
  		myBufferVals.lightDiffuse,
  		myBufferVals.lightSpecular 
    );
    vec4 finalColor=gzqd*textureLod(tex, vTexCoor, 0.0);
    outColor = (1.0-fxlFlag)*gzqd*finalColor+fxlFlag*vec4(0.1,0.4,0.01,1);
}