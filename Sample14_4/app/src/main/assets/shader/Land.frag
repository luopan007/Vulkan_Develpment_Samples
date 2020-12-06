#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 1) uniform sampler2D sTexture;
layout (std140,set = 0, binding = 2) uniform bufferValsFrag {
    mat4 uMVPMatrixGY;//投影贴图用摄像机及投影综合矩阵
} myBufferValsFrag;
layout (location = 0) in vec4 inLightQD;
layout (location = 1) in vec4 vPosition;
layout (location = 0) out vec4 outColor;

void main(){
   //将片元的位置投影到光源处虚拟摄像机的近平面上
   vec4 gytyPosition=myBufferValsFrag.uMVPMatrixGY * vec4(vPosition.xyz,1);
   gytyPosition=gytyPosition/gytyPosition.w;	//进行透视除法
   float s=(gytyPosition.x+1.0)/2.0;				//将投影后的坐标换算为纹理坐标
   float t=(gytyPosition.y+1.0)/2.0;
   vec4 finalcolor=vec4(0.8,0.8,0.8,1.0); 		   //物体本身的颜色
   vec4 colorA=finalcolor*inLightQD;//光照下颜色
   vec4 colorB=vec4(0.1,0.1,0.1,0.0);//阴影下的颜色
   if(s>=0.0&&s<=1.0&&t>=0.0&&t<=1.0)
   {
        //若纹理坐标在合法范围内则考虑投影贴图
		vec4 projColor=texture(sTexture, vec2(s,t)); //对投影纹理图进行采样
		float a=step(0.9999,projColor.r);								//如果r<0.9999，则a=0，否则a=1
        float b=step(0.0001,projColor.r); 								//如果r<0.0001，则a=b，否则b=1
        float c=1.0-sign(a);  											//如果a>0,则c=1.如果a=0，则c=0
		outColor =a*colorA+(1.0-b)*colorB+b*c*mix(colorB,colorA,smoothstep(0.0,1.0,projColor.r));					//计算最终片元颜色

   }
   else
   {
         outColor = colorA;
   }


}