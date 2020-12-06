#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D texDepth; //纹理采样器（代表距离纹理）
layout (std140,set = 0, binding = 2) uniform bufferValsFrag {
    mat4 uMVPMatrixGY; //投影贴图用投影与观察组合矩阵
    float near; //near 面距离
    float far; //far 面距离
} myBufferValsFrag;
layout (binding = 3) uniform sampler2D texHW; //纹理采样器（代表物体本身纹理）
layout (location = 0) in vec2 inTexCoor; //接收的顶点纹理坐标
layout (location = 1) in vec4 inLightQD; //接收的光照强度
layout (location = 2) in vec4 vPosition; //接收的顶点坐标
layout (location = 3) in vec3 vNormal; //接收的法向量
layout (location = 4) in vec3 vlightPosition; //接收的光源位置
layout (location = 5) in vec4 vlightAmbient; //接收的环境光强度
layout (location = 0) out vec4 outColor; //输出到管线的片元颜色
float from4To1(vec4 color){ //将四通道值组装为 0~2^32 之间的距离值
    float result=0.0; //距离值结果
    result=result+color.r*256.0+color.g*65536.0+color.b*16777216.0+color.a*4294967296.0;//计算距离值
    return result; //返回距离值
}
float fromDisToZ(float dis,float near,float far){ //将 near 到 far 范围内的距离值压缩到 0～1 之间的方法
    return (dis-near)/(far-near);
}
void main(){ //主方法
    float depthReal=fromDisToZ(distance(vPosition.xyz,vlightPosition)-2.0,//计算当前片元到光源的距离
        myBufferValsFrag.near,myBufferValsFrag.far);
    vec4 gytyPosition=myBufferValsFrag.uMVPMatrixGY * //将片元位置通过矩阵变换进剪裁空间中
    vec4(vPosition.xyz,1.0);
    gytyPosition=gytyPosition/gytyPosition.w; //执行透视除法将片元位置变换进标准设备空间中
    float s=(gytyPosition.x+1.0)/2.0; //将标准设备空间中 XOY 平面内的 X 坐标变换为纹理 S 坐标
    float t=1.0-(gytyPosition.y+1.0)/2.0; //将标准设备空间中 XOY 平面内的 Y 坐标变换为纹理 T 坐标
    vec4 finalColor=texture(texHW,inTexCoor); //采样出物体本身的颜色
    if(s>=0.0&&s<=1.0&&t>=0.0&&t<=1.0){ //若纹理坐标在合法范围内则考虑投影贴图
        vec4 projColor=texture (texDepth,vec2(s,t)); //对投影纹理图进行采样
        float depth=from4To1(projColor)/4294967296.0; //将纹理采样值换算成 0～1 之间的距离值
        if(depth<=depthReal){ //若当前片元距离光源的距离大于记录的最小距离
            outColor=finalColor*vlightAmbient; //结合环境光强度计算最终片元颜色值（在阴影中）
        }else{ //若当前片元距离光源的距离不大于记录的最小距离
            outColor = finalColor*inLightQD; //结合光照强度计算最终片元颜色值（不在阴影中）
    }}else { //若纹理坐标不在合法范围内则不考虑投影贴图
        outColor = finalColor*inLightQD; //结合光照强度计算最终片元颜色值（不在阴影中）
}}
