#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) out vec4 outColor;
layout (location = 0) in vec4 inLightQD;
layout (location = 1) in float vFogFactor;
void main()
{
    vec4 objectColor=vec4(0.95,0.95,0.95,1.0);
    vec4 fogColor = vec4(0.97,0.76,0.03,1.0);
     if(vFogFactor != 0.0)
     {
    	objectColor = objectColor*inLightQD;
    	outColor = objectColor*vFogFactor + fogColor*(1.0-vFogFactor);
     }else
     {
     	   outColor=fogColor;
     }
}