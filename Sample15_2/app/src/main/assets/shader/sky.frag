#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

//目前的实验其表明 必须是4的整数倍 否则奇怪现象
layout (std140,set = 0, binding = 0) uniform bufferVals {
     float brightFactor;
} myBufferVals;

layout (binding = 1) uniform sampler2D tex;
layout (location = 0) in vec2 inTexCoor;
layout (location = 0) out vec4 outColor;
layout (location = 1) out float outDepth;


void main() {
   outColor=myBufferVals.brightFactor*texture(tex, inTexCoor);
   outDepth=gl_FragCoord.z;
}