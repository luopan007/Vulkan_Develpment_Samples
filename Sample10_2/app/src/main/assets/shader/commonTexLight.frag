#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex;
layout (location = 0) in vec2 inTexCoor;
layout (location = 1) in vec4 inLightQD;
layout (location = 0) out vec4 outColor;
void main() {
    vec4 cyz=textureLod(tex, inTexCoor, 0.0);
    outColor=vec4(inLightQD.rgb*cyz.rgb,cyz.a);
}