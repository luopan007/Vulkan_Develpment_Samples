#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D sTexture; //纹理采样器（阴影贴图）
layout (std140,set = 0, binding = 2) uniform bufferValsFrag { //一致变量块
    mat4 uMVPMatrixGY; //投影贴图用投影与观察组合矩阵
} myBufferValsFrag;
layout (location = 0) in vec4 inLightQD; //接收从顶点着色器传递过来的光照强度
layout (location = 1) in vec4 vPosition; //接收从顶点着色器传递过来的片元位置
layout (location = 2) in float isShadow; //接收从顶点着色器传递过来的阴影绘制标志
layout (location = 3) in vec4 vlightAmbient; //接收从顶点着色器传递过来的环境光参数
layout (location = 4) in vec4 vlightDiffuse; //接收从顶点着色器传递过来的散射光参数
layout (location = 0) out vec4 outColor; //输出到管线的片元最终颜色
void main(){
    if(isShadow==0){ //若不是绘制阴影，将片元的位置投影到光源处虚拟摄像机的近平面上
        vec4 gytyPosition=myBufferValsFrag.uMVPMatrixGY *
            vec4(vPosition.xyz,1); //将片元位置通过矩阵变换进剪裁空间中
        gytyPosition=gytyPosition/gytyPosition.w; //执行透视除法将片元位置变换进标准设备空间中
        float s=(gytyPosition.x+1.0)/2.0; //将标准设备空间中 XOY 平面内的 X 坐标变换为纹理 S 坐标
        float t=(gytyPosition.y+1.0)/2.0; //将标准设备空间中 XOY 平面内的 Y 坐标变换为纹理 T 坐标
        vec4 finalcolor=vec4(0.8,0.8,0.8,1.0); //物体本身的颜色
        vec4 colorA=finalcolor*inLightQD; //光照下颜色
        //计算在阴影中的片元颜色，此时仅有环境光、散射光，而且散射光减弱为原来的 30%
        vec4 colorB=finalcolor*vlightAmbient+finalcolor*vlightDiffuse*0.3;
        if(s>=0.0&&s<=1.0&&t>=0.0&&t<=1.0){ //若纹理坐标在合法范围内则考虑投影贴图
            vec4 projColor=texture(sTexture, vec2(s,t)); //对投影纹理图进行采样
            float a=step(0.9999,projColor.r); //如果 r<0.9999，则 a=0，否则 a=1
            float b=step(0.0001,projColor.r); //如果 r<0.0001，则 b=0，否则 b=1
            float c=1.0-sign(a); //如果 a>0,则 c=1.如果 a=0，则 c=0
            outColor =a*colorA+(1.0-b)*colorB+b*c*mix(colorB,colorA,
                smoothstep(0.0,1.0,projColor.r)); //计算最终片元颜色
        }else{outColor = colorB; } //若纹理坐标不在合理范围内，则颜色为阴影内颜色
    }else{ //若是绘制阴影
        outColor=vec4(0.1,0.1,0.1,1.0); //片元最终颜色为阴影的颜色
    }
}
