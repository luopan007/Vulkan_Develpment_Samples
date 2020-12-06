#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals {
	layout(offset=128) vec4 color;
} myConstantValsFrag;
layout (binding = 1) uniform sampler2D tex;
layout (location = 0) in vec4 inLightQD;
layout (location = 0) out vec4 outColor;
void main() {
    outColor=inLightQD*myConstantValsFrag.color;
}