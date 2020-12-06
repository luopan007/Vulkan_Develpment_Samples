#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals {
	mat4 mvp;
	mat4 mm;
} myConstantVals;
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTexCoor;
layout (location = 2) in vec3 inNormal;
layout (location = 0) out vec2 outTexCoor;
layout (location = 1) out vec3 outPos;
layout (location = 2) out vec3 outNormal;
out gl_PerVertex { 
	vec4 gl_Position;
};
vec3 normalFromObjectToWorld
(
   in mat4 uMMatrix,             
   in vec3 normal,				 
   in vec3 position				 
)
{
	vec3 normalTarget=position+normal;	
    vec3 newNormal=(uMMatrix*vec4(normalTarget,1)).xyz-(uMMatrix*vec4(position,1)).xyz;
    newNormal=normalize(newNormal); 	
	return newNormal;
}
void main() {
    outTexCoor = inTexCoor;
	outNormal = normalFromObjectToWorld(myConstantVals.mm,inNormal,pos);
	outPos =(myConstantVals.mm*vec4(pos,1)).xyz;
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);
}
