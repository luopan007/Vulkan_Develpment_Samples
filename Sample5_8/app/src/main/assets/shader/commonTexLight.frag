#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec4 inLightQD;
layout (location = 1) in vec3 vposition;
layout (location = 0) out vec4 outColor;

vec4 genBoardColor(vec3 position)
{
   const float R=1.0;
   vec4 color;
   float n = 8.0;
   float span = 2.0*R/n;
   int i = int((position.x + 1.0)/span);
   int j = int((position.y + 1.0)/span);
   int k = int((position.z + 1.0)/span);
   int whichColor = int(mod(float(i+j+k),2.0));
   if(whichColor == 1) {
   		color = vec4(0.678,0.231,0.129,1.0);
   }
   else {
   		color = vec4(1.0,1.0,1.0,1.0);
   }
   return color;
}

void main() {
   outColor=inLightQD*genBoardColor(vposition);
}