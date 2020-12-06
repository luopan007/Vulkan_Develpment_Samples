#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals {//一致变量块
    float isBloom;//是否启用泛光效果标志
} myBufferVals;
layout (binding = 1) uniform sampler2D Tex;//纹理采样器(正常绘制的场景纹理)
layout (binding = 2) uniform sampler2D BloomTex;//纹理采样器(发光部分纹理)
layout (location = 0) in vec2 inTexCoor;//接收从顶点着色器过来的纹理坐标
layout (location = 0) out vec4 outColor;//输出的片元颜色
vec4 gaussBlur(vec2 stCoord)//实现高斯模糊采样的方法
{
	const float stStep = 256.0;
    const float scaleFactor = 1.0/273.0;
    vec2 offsets[25]=vec2[25]
    (
      vec2(-2.0,-2.0),vec2(-1.0,-2.0),vec2(0.0,-2.0),vec2(1.0,-2.0),vec2(2.0,-2.0),
      vec2(-2.0,-1.0),vec2(-1.0,-1.0),vec2(0.0,-1.0),vec2(1.0,-1.0),vec2(2.0,-1.0),
      vec2(-2.0,0.0),vec2(-1.0,0.0),vec2(0.0,0.0),vec2(1.0,0.0),vec2(2.0,0.0),
      vec2(-2.0,1.0),vec2(-1.0,1.0),vec2(0.0,1.0),vec2(1.0,1.0),vec2(2.0,1.0),
      vec2(-2.0,2.0),vec2(-1.0,2.0),vec2(0.0,2.0),vec2(1.0,2.0),vec2(2.0,2.0)
    );
    float kernelValues[25]=float[25]
    (
        1.0,4.0,7.0,4.0,1.0,
        4.0,16.0,26.0,16.0,4.0,
        7.0,26.0,41.0,26.0,7.0,
        4.0,16.0,26.0,16.0,4.0,
        1.0,4.0,7.0,4.0,1.0
    );
    vec4 sum=vec4(0,0,0,0);
    for(int i=0;i<25;i++)
    {
        sum=sum+kernelValues[i]*texture(BloomTex, stCoord+offsets[i]/stStep);
    }
    return sum*scaleFactor;
}
void main() {//主方法
    if(myBufferVals.isBloom==0){//若启用泛光效果
        vec4 hdrColor = texture(Tex, inTexCoor);//对正常绘制得到的场景纹理1 采样得到颜色值1
        vec4 bloomColor = gaussBlur(inTexCoor);//对发光部分纹理2 进行高斯模糊采样得到颜色值2
        hdrColor += bloomColor; //将上述两个颜色值相加
        outColor = hdrColor;//输出结果
    }else{//若不启用泛光效果
        outColor=texture(Tex, inTexCoor);//结果颜色值为对正常绘制得到的场景纹理1 采样得到的颜色值
    }
}
