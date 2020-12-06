#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D texDepth;//距离纹理
layout (std140,set = 0, binding = 2) uniform bufferValsFrag {
    mat4 uMVPMatrixGY;//投影贴图用摄像机及投影综合矩阵
	float near;
	float far;
} myBufferValsFrag;
layout (binding = 3) uniform sampler2D texHW;//物体本身纹理
layout (location = 0) in vec2 inTexCoor;
layout (location = 1) in vec4 inLightQD;
layout (location = 2) in vec4 vPosition;
layout (location = 3) in vec3 vNormal;
layout (location = 4) in vec3 vlightPosition;
layout (location = 5) in vec4 vlightAmbient;
layout (location = 0) out vec4 outColor;
//将四通道值组装为0~2^32之间的距离值
float from4To1(vec4 color)
{
    float result=0.0;
    result=result+color.r*256.0+color.g*65536.0+color.b*16777216.0+color.a*4294967296.0;
    return result;
}
//将near到far范围内的距离压缩到0～1之间
//压缩范围的好处是提高比较的精度
float fromDisToZ(float dis,float near,float far)
{
   return (dis-near)/(far-near);
}
void main(){ //主方法
    float depthReal=fromDisToZ(distance(vPosition.xyz,vlightPosition)-10.0, //计算当前片元到光源的距离
        myBufferValsFrag.near,myBufferValsFrag.far);
    vec4 gytyPosition=myBufferValsFrag.uMVPMatrixGY * vec4(vPosition.xyz,1.0);
    gytyPosition=gytyPosition/gytyPosition.w; //将片元位置通过矩阵变换进剪裁空间中
    float s=(gytyPosition.s+1.0)/2.0; //将标准设备空间中 XOY 平面内的 X 坐标变换为纹理 S 坐标
    float t=1.0-(gytyPosition.t+1.0)/2.0; //将标准设备空间中 XOY 平面内的 Y 坐标变换为纹理 T 坐标
    vec4 finalColor=texture (texHW,inTexCoor); //物体本身的颜色
    if(s>=0.0&&s<=1.0&&t>=0.0&&t<=1.0){ //若纹理坐标在合法范围内则考虑投影贴图
        vec4 projColor=texture(texDepth,vec2(s,t)); //对投影纹理图进行采样
        float depth=from4To1(projColor)/4294967296.0; //将纹理采样值换算成 0～1 之间的距离值
        const float p=0.0008; //阴影过渡的阈值
        const float doublep=0.0016; //阴影过渡区域的长度
        if(depth<=depthReal){ //若当前片元距离光源的距离大于记录的最小距离
            outColor=finalColor*vlightAmbient; //结合环境光强度计算最终片元颜色值
        }else if(depth>=depthReal+p){ //若最小距离大于当前片元到光源的距离与阈值的和用光照强度着色
            outColor = finalColor*inLightQD; //结合光照强度计算最终片元颜色值
        }else{ //如果在阴影过渡区域内
            vec4 color=finalColor*vlightAmbient; //结合环境光强度计算最终片元颜色值
            vec4 color2=finalColor*inLightQD; //结合光照强度计算最终片元颜色值
            float b=(depthReal+p-depth)/doublep; //计算当前距离所占长度的比例
            outColor=b*color+(1.0-b)*color2; //计算阴影过渡的颜色值
    }}else { //计算最终片元的颜色
        outColor = finalColor*inLightQD; //结合光照强度计算最终片元颜色值
}}









