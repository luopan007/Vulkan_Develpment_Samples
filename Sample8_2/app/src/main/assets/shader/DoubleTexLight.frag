#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 1) uniform sampler2D texDay;
layout (binding = 2) uniform sampler2D texNight;

layout (location = 0) in vec2 inTexCoor;
layout (location = 1) in vec4 inLightQD;

layout (location = 0) out vec4 outColor;

void main() {
   //outColor=inLightQD*textureLod(texNight, inTexCoor, 0.0);

   //从白天纹理中采样出颜色值并结合光照强度
   vec4 finalColorDay=inLightQD*textureLod(texDay, inTexCoor, 0.0);
   //从夜晚纹理中采样出颜色值
   vec4 finalColorNight=vec4(0.5,0.5,0.5,1.0)*textureLod(texNight, inTexCoor, 0.0);

    if(inLightQD.x>0.21)
    {//当光照强度大于0.21时，采用白天
        outColor=finalColorDay;
    }
    else if(inLightQD.x<0.05)
    {//当光照强度小于0.05时,采用夜间
        outColor=finalColorNight;
    }
    else
    {//当光照强度大于0.05小于0.21时，为白天夜间的过渡阶段
        float t=(inLightQD.x-0.05)/0.16;//计算白天纹理应占纹理过渡阶段的百分比
        outColor=t*finalColorDay+(1.0-t)*finalColorNight;//计算白天黑夜过渡阶段的颜色值
    }
}