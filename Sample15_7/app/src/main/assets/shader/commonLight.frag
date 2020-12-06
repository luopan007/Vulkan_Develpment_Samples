#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex;//纹理采样器，代表一幅纹理
layout (location = 0) in vec4 inLightQD; //接收的光照强度
layout (location = 1) in float shadow; //接收的阴影绘制标志
layout (location = 2) in vec4 lightPosition; //接收从顶点着色器传递过来的光源位置
layout (location = 3) in vec3 vPosition; //接收从顶点着色器传递过来的片元位置
layout (location = 4) in vec3 vNormal; //接收从顶点着色器传递过来的法向量
layout (location = 5) in vec3 vlight; //接收从顶点着色器传递过来的聚光灯方向向量
layout (location = 6) in vec2 inTexCoor;//接收从顶点着色器传递过来的纹理坐标
layout (location = 7) in float inToneMapping;//接收的色调映射启用标志值
layout (location = 0) out vec4 outColor; //输出到管线的片元颜色
void main(){ //主方法
    const vec4 colorB=vec4(0.1,0.1,0.1,1.0); //物体在非光照区的颜色(阴影颜色)
    if(shadow==0){ //绘制物体本身
        vec3 L=normalize(vPosition-lightPosition.xyz); //光源到被照射点位置的向量
        float qdFactor=0.0; //聚光灯用光照强度变化系数
        const float thetaU=10.0; //聚光灯截止角
        const float thetaUCos=cos(thetaU*3.1415927/180.0);//聚光灯截止角余弦
        const float Sexp=5.0; //光照强度减弱程度控制系数(值越大越平滑)
        vec3 vlightN=normalize(vlight); //聚光灯方向向量规格化
        float cosThetaS=dot(vlightN,L); //光源到被照射位置向量与聚光灯方向向量夹角余弦
        if(cosThetaS<thetaUCos){ //光源到被照射位置向量与聚光灯方向向量夹角大于截止角
            qdFactor=0; //聚光灯用光照强度变化系数为 0
        }else{ //光源到被照射位置向量与聚光灯方向向量夹角小于截止角
            qdFactor= pow (cosThetaS,Sexp); //计算聚光灯用光照强度变化系数
        }
        const vec4 objColor=texture(tex, inTexCoor); //物体本身的颜色
        vec4 colorA=objColor*inLightQD; //计算物体正常光照下的颜色(不考虑聚光灯)
        if(qdFactor==0){ //若在阴影区
            outColor=colorB; //片元颜色为物体在非光照区的颜色
        }else{ //若不在阴影区
            if(inToneMapping==0){//若采用色调映射
                vec4 hdrColor = colorA*qdFactor;//物体本身的颜色乘以聚光灯用光照强度变化系数
                vec3 mapped = hdrColor.rgb / (hdrColor.rgb + vec3(1.0));//richard 色调映射计算
                outColor = vec4(mapped, 1.0);//片元最终颜色
            }else{//若不采用色调映射
                outColor=colorA*qdFactor; //物体本身的颜色乘以聚光灯用光照强度变化系数
            }
            if(length(outColor.rgb)<length(colorB.rgb)){//若衰减后的亮度小于场景环境光最低亮度
                outColor=colorB; //片元颜色为物体在非光照区的颜色
    }}
    }else{ //绘制阴影
        outColor = colorB; //片元最终颜色为阴影的颜色
}}
