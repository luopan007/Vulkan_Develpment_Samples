#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tCaodi;
layout (binding = 2) uniform sampler2D tSand;
layout (location = 0) in vec2 inTexCoor;
layout (location = 1) in float landHeight;

layout (location = 0) out vec4 outColor;
void main() {
    float height1=90;
    float height2=120;
    vec4  colorCaodi=textureLod(tCaodi, inTexCoor, 0.0);//采样出草地颜色
    vec4 colorSand=textureLod(tSand, inTexCoor, 0.0);//采样出沙滩颜色
    if(landHeight<height1){//绘制沙滩
    outColor=colorSand;
    }else if(landHeight<height2){//绘制混合颜色
    float radio=(landHeight-height1)/(height2-height1);
    outColor=mix(colorCaodi,colorSand,1-radio);
    }else{//绘制草地
    outColor=colorCaodi;
    }
}