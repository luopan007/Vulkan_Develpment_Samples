#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex; //纹理采样器（物体本身纹理）
layout (binding = 2) uniform sampler2D texBloom;//纹理采样器（发光部分标识纹理）
layout (location = 0) in vec2 inTexCoor; //从顶点着色器传入的纹理坐标
layout (location = 1) in vec4 inLightQD;//从顶点着色器传入的光照强度
layout (location = 0) out vec4 outColor; //输出到管线的片元颜色值
layout (location = 1) out vec4 BrightColor ; //输出到管线的发光片元颜色值
void main() { //主方法
    vec4 finalColor=inLightQD*texture(tex, inTexCoor); //计算片元的最终颜色值
    outColor=finalColor; //输出到管线的片元颜色值
	vec4 bloomColor=texture(texBloom, inTexCoor);//采样获取发光部分标识纹理的颜色值
    if(bloomColor==vec4(1.0,1.0,1.0,1.0)){//若此片元颜色值为白色(片元位于发光部分)
        BrightColor= outColor;//输出到管线的发光片元颜色值为片元的最终颜色值
    }else{//若此片元颜色值为黑色(片元不位于发光部分)
        BrightColor=vec4(0.0,0.0,0.0, 1.0);//输出到管线的发光片元颜色值为黑色
    }
}
