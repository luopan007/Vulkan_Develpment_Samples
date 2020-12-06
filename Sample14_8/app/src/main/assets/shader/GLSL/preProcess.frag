#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 0) uniform sampler2D tex;
layout (location = 0) in vec2 inTexCoor;
layout (location = 1) in vec3 inPos;
layout (location = 2) in vec3 inNormal;
layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outPos;
layout (location = 2) out vec4 outNormal;
void main() {   
   outColor=texture(tex, inTexCoor);
   outPos=vec4(inPos.xyz,1.0);
   outNormal=vec4(inNormal.xyz,1.0);
}
