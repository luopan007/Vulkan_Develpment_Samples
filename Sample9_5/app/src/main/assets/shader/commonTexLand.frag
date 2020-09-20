#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

//目前的实验其表明 必须是4的整数倍 否则奇怪现象
layout (std140,set = 0, binding = 0) uniform bufferVals {
    float brightFactor;
    float landStartY;
    float landYSpan;
} myBufferVals;

layout (binding = 1) uniform sampler2D texGrass;
layout (binding = 2) uniform sampler2D texRock;

layout (location = 0) in vec2 inTexCoor;
layout (location = 1) in float landHeight;
layout (location = 0) out vec4 outColor;

void main() {

    vec4 grassColor = textureLod(texGrass, inTexCoor, 0.0);
    vec4 rockColor = textureLod(texRock, inTexCoor, 0.0);
    vec4 finalColor;

    if(landHeight < myBufferVals.landStartY)
    {
        finalColor = grassColor;
    }else if(landHeight > myBufferVals.landStartY + myBufferVals.landYSpan)
    {
        finalColor = rockColor;
    }else
    {
        float currYRatio = (landHeight - myBufferVals.landStartY) / myBufferVals.landYSpan;
        finalColor = currYRatio * rockColor + (1.0 - currYRatio) * grassColor;
    }

   outColor=myBufferVals.brightFactor*finalColor;

}