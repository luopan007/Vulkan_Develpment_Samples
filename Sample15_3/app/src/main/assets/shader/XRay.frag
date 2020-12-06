#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals {
    vec4 uCamera;//摄像机位置
    float screenWidth;//屏幕宽度
    float screenHeight;//屏幕高度
} myBufferVals;
layout (binding = 1) uniform sampler2D texScene;
layout (binding = 2) uniform sampler2D texDepth;
layout (binding = 3) uniform sampler2D texPeople;
layout (location = 0) in vec4 vPosition;
layout (location = 0) out vec4 outColor;
void main() {
    float dis=distance(vPosition.xyz,myBufferVals.uCamera.xyz);//计算摄像机与顶点距离
    float s=gl_FragCoord.x/myBufferVals.screenWidth;//换算纹理坐标
    float t=gl_FragCoord.y/myBufferVals.screenHeight;//换算纹理坐标
    vec2 stCurr=vec2(s,t);//得到纹理坐标
    float depth=texture(texDepth, stCurr).r;//采样出深度
    vec4 peopleColor=texture(texPeople, stCurr);//采样正常人物纹理（正常人物）
    vec4 sceneColor=texture(texScene, stCurr);//采样出正常场景纹理（无人物）
    if(dis>depth){//挡住了
       float realFactor=(sceneColor.r+sceneColor.g+sceneColor.b)/3.0;
       vec4 realGray=vec4(realFactor,realFactor,realFactor,1.0);//计算场景的灰度值
       float peopleFactor=(peopleColor.r+peopleColor.g+peopleColor.b)/3.0;
       vec4 peopleGray=vec4(peopleFactor,peopleFactor,peopleFactor,1.0);//计算人物的灰度值
       outColor=mix(realGray,peopleGray,0.3);//混合颜色
    }else{
        outColor=peopleColor;//人物颜色
    }
}