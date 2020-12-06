#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals {//一致变量块
    vec4 uCamera;//摄像机位置
    float blurWidth;//过渡区域的范围
    float blurPosition;//过渡区域的开始位置
    float screenWidth;//以像素计的视口宽度(全屏情况下即屏幕宽度)
    float screenHeight;//以像素计的视口高度(全屏情况下即屏幕高度)
} myBufferVals;
layout (binding = 1) uniform sampler2D tex;//纹理采样器(第一轮绘制生成的纹理)
layout (location = 0) in vec4 vPosition;//接收的顶点位置
layout (location = 0) out vec4 outColor;//输出到管线的片元颜色
vec4 gaussBlur(vec2 stCoord)//实现高斯模糊采样的方法
{
	const float stStep = 512.0;//纹理偏移量单位步进调整因子
    const float scaleFactor = 1.0/273.0;//高斯模糊求和时的加权因子(为调整亮度)
    vec2 offsets[25]=vec2[25]//给出卷积内核中各个元素对应像素相对于待处理像素的纹理坐标偏移量
    (
      vec2(-2.0,-2.0),vec2(-1.0,-2.0),vec2(0.0,-2.0),vec2(1.0,-2.0),vec2(2.0,-2.0),
      vec2(-2.0,-1.0),vec2(-1.0,-1.0),vec2(0.0,-1.0),vec2(1.0,-1.0),vec2(2.0,-1.0),
      vec2(-2.0,0.0),vec2(-1.0,0.0),vec2(0.0,0.0),vec2(1.0,0.0),vec2(2.0,0.0),
      vec2(-2.0,1.0),vec2(-1.0,1.0),vec2(0.0,1.0),vec2(1.0,1.0),vec2(2.0,1.0),
      vec2(-2.0,2.0),vec2(-1.0,2.0),vec2(0.0,2.0),vec2(1.0,2.0),vec2(2.0,2.0)
    );
    float kernelValues[25]=float[25]//卷积内核中各个位置的权值
    (
        1.0,4.0,7.0,4.0,1.0,
        4.0,16.0,26.0,16.0,4.0,
        7.0,26.0,41.0,26.0,7.0,
        4.0,16.0,26.0,16.0,4.0,
        1.0,4.0,7.0,4.0,1.0
    );
    vec4 sum=vec4(0,0,0,0);//用于记录结果颜色和的变量
    for(int i=0;i<25;i++)//遍历卷积内核各元素
    {
        sum=sum+kernelValues[i]*texture(tex, stCoord+offsets[i]/stStep);//进行纹理采样并求和
    }
    return sum*scaleFactor;//返回高斯模糊采样的颜色值
}
void main()//主方法
{
    float dis=distance(vPosition.xyz,myBufferVals.uCamera.xyz);//计算摄像机与顶点距离
    float s=gl_FragCoord.x/myBufferVals.screenWidth;//换算出纹理坐标S
    float t=gl_FragCoord.y/myBufferVals.screenHeight;//换算出纹理坐标T
    vec2 stCurr=vec2(s,t);//构建纹理坐标
    float factor=(dis-myBufferVals.blurPosition)/myBufferVals.blurWidth;//清晰区域值小于0，过渡区域值为0~1，模糊区域值大于1
    factor=smoothstep(0.0,1.0,factor);//将距离因子值平滑处理为0～1 之间
    vec4 vividColor=texture(tex, stCurr);//清晰区域的颜色值（直接进行采样）
    vec4 blurColor=gaussBlur(stCurr);//模糊区域的颜色值（通过自定义高斯采样函数采样）
    outColor=mix(vividColor,blurColor,factor);
}
