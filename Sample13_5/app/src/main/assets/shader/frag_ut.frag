#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals {
    vec4 uCamera;
	vec4 lightPosition;
	vec4 lightAmbient;
	vec4 lightDiffuse;
	vec4 lightSpecular;
} myBufferVals;
layout (push_constant) uniform constantVals { //推送常量
    layout(offset=64) mat4 mm; //基本变换矩阵
} myConstantValsFrag;
layout (binding = 1) uniform sampler2D sTextureWg; //纹理内容数据（外观）
layout (binding = 2) uniform sampler2D sTextureNormal; //纹理内容数据（法向量）
layout (location = 0) in vec3 vPosition; //接收从顶点着色器传递过来的顶点坐标
layout (location = 1) in vec2 vTextureCoord; //接收从顶点着色器传递过来的纹理坐标
layout (location = 2) in vec3 fNormal; //接收从顶点着色器传递过来的法向量
layout (location = 3) in vec3 ftNormal; //接收从顶点着色器传递过来的切向量
layout (location = 0) out vec4 outColor; //输出到渲染管线的最终片元颜色
vec4 pointLight( //定位光光照计算的方法
in vec3 normal, //扰动后的法向量
in vec3 vp, //变换到标准法向量所属坐标系的表面点到光源位置的向量
in vec3 eye, //变换到标准法向量所属坐标系的视线向量
in vec4 lightAmbient, //环境光强度
in vec4 lightDiffuse, //散射光强度
in vec4 lightSpecular //镜面光强度
){
    vec4 diffuse;vec4 specular; //散射光、镜面光结果强度
    vec3 halfVector=normalize(vp+eye); //求视线与光线的半向量
    float shininess=50.0; //粗糙度，越小越光滑
    float nDotViewPosition=max(0.0,dot(normal,vp)); //求法向量与vp 的点积与0 的最大值
    diffuse=lightDiffuse*nDotViewPosition; //计算散射光的最终强度
    float nDotViewHalfVector=dot(normal,halfVector); //法向量与半向量的点积
    float powerFactor=max(0.0,pow(nDotViewHalfVector,shininess)); //镜面反射光强度因子
    specular=lightSpecular*powerFactor; //计算镜面光的最终强度
    return lightAmbient+diffuse+specular; //返回结果光照强度
}
void main() {
    vec4 normalColor = texture(sTextureNormal, vTextureCoord); //从法线纹理图中读出值
    vec3 cNormal=vec3(2.0*(normalColor.r-0.5),2.0 //将值恢复到-1～+1 范围
    *(normalColor.g-0.5),2.0*(normalColor.b-0.5));
    cNormal=normalize(cNormal); //将扰动结果向量规格化
    vec3 normalTarget=vPosition+fNormal; //计算世界坐标系法向量
    vec3 newNormal=(myConstantValsFrag.mm
    *vec4(normalTarget,1)).xyz-(myConstantValsFrag.mm*vec4(vPosition,1)).xyz;
    newNormal=normalize(newNormal); //规格化法向量
    vec3 tangentTarget=vPosition+ftNormal; //计算变换后的切向量
    vec3 newTangent=(myConstantValsFrag.mm
    *vec4(tangentTarget,1)).xyz-(myConstantValsFrag.mm*vec4(vPosition,1)).xyz;
    newTangent=normalize(newTangent);
    vec3 binormal=normalize(cross(newTangent,newNormal));
    mat3 rotation=mat3(newTangent,binormal,newNormal); //组装计算矩阵
    vec3 newPosition=(myConstantValsFrag.mm*vec4(vPosition,1)).xyz;//变换后的片元位置
    vec3 vp= normalize(myBufferVals.lightPosition.xyz-newPosition);//求表面点到光源位置的向量vp
    vp=normalize(rotation*vp); //变换并规格化vp 向量
    vec3 eye= normalize(rotation* //求出从表面点到摄像机的视线向量进行变换并规格化
    normalize(myBufferVals.uCamera.xyz-newPosition));
    vec4 LightQD=pointLight(cNormal,vp,eye, //计算光照强度
    myBufferVals.lightAmbient,myBufferVals.lightDiffuse,myBufferVals.lightSpecular);
    vec4 finalColor=texture(sTextureWg, vTextureCoord); //根据纹理坐标采样出片元颜色
    outColor=LightQD*finalColor; //得到最终片元颜色
}
