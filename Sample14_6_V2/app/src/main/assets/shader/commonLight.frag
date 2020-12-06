#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec4 inLightQD;
layout (location = 1) in float shadow;
layout (location = 2) in vec4 lightPosition;
layout (location = 3) in vec3 vPosition;
layout (location = 4) in vec3 vNormal;
layout (location = 5) in vec3 vlight;//聚光灯方向向量
layout (location = 0) out vec4 outColor;
void main(){
   const vec4 colorB=vec4(0.05,0.05,0.05,1.0);//物体在非光照区的颜色
   if(shadow==0)//绘制物体本身
   {
        //光源到被照射点位置的向量
       	vec3 L=normalize(vPosition-lightPosition.xyz);
       	//聚光灯用光照强度变化系数
       	float qdFactor=0.0;
        //聚光灯截止角
        const float thetaU=20.0;
        //聚光灯截止角余弦
       	const float thetaUCos=cos(thetaU*3.1415927/180.0);
       	//聚光灯半影区起始角
       	const float thetaP=15.0;
       	//聚光灯半影区起始角余弦
       	const float thetaPCos=cos(thetaP*3.1415927/180.0);
       	//光照强度减弱程度控制系数(值越大衰减越平滑)
       	const float Sexp=3.0;
       	//聚光灯方向向量规格化
       	vec3 vlightN=normalize(vlight);
       	//光源到被照射位置向量与聚光灯方向向量夹角余弦
       	float cosThetaS=dot(vlightN,L);
       	if(cosThetaS>thetaPCos)
       	{//若在聚光灯光锥最中间区域
       	    qdFactor=1.0;//聚光灯用光照强度变化系数为 1
       	}
       	else if(cosThetaS>thetaUCos)
       	{//若光源到被照射位置向量与聚光灯方向向量夹角在半影区域内
       	    float temp=(cosThetaS-thetaUCos)/(thetaPCos-thetaUCos);
       	    qdFactor= pow (temp,Sexp);//计算聚光灯用光照强度变化系数
       	}
	    else
       	{//若光源到被照射位置向量与聚光灯方向向量夹角大于截止角
       	    qdFactor=0.0;//聚光灯用光照强度变化系数为 0
       	}
        const vec4 objColor=vec4(0.8,0.8,0.8,1.0);//物体本身的颜色
        vec4 colorA=objColor*inLightQD;//计算物体正常光照下的颜色(不考虑聚光灯)
    	if(qdFactor==0)
       	{//若在阴影区
       	  	outColor=colorB;
       	}
       	else
       	{//若不在阴影区
           	outColor=colorA*qdFactor;
           	//若衰减后的亮度小于场景环境光最低亮度
           	if(length(outColor.rgb)<length(colorB.rgb))
           	{
           	    outColor=colorB;
           	}
       	}
   	}else{	//绘制阴影
	    outColor = colorB;		//片元最终颜色为阴影的颜色
    }
}