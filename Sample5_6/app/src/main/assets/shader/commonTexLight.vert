#version 400
#extension GL_ARB_separate_shader_objects : enable //开启separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable //开启shading_language_420pack
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致块
    vec4 uCamera; //摄像机位置
    vec4 uLightDirection; //光源方向向量
    vec4 lightAmbient; //环境光强度
    vec4 lightDiffuse; //散射光强度
    vec4 lightSpecular; //镜面光强度
} myBufferVals;
layout (push_constant) uniform constantVals{ //推送常量块
mat4 mvp; //最终变换矩阵
mat4 mm; //基本变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec3 inNormal; //输入的法向量
layout (location = 0) out vec4 outLightQD; //输出的光照强度
layout (location = 1) out vec3 vposition; //输出的顶点位置
out gl_PerVertex { //输出接口块
vec4 gl_Position; //内建变量gl_Position
};
vec4 directionalLight ( //定向光光照计算的方法
    in mat4 uMMatrix, //基本变换矩阵
    in vec3 uCamera, //摄像机位置
    in vec3 lightDirection, //定向光方向
    in vec4 lightAmbient, //环境光强度
    in vec4 lightDiffuse, //散射光强度
    in vec4 lightSpecular, //镜面光强度
    in vec3 normal, //法向量
    in vec3 aPosition //顶点位置
){
    vec4 ambient; //环境光最终强度
    vec4 diffuse; //散射光最终强度
    vec4 specular; //镜面光最终强度
    ambient=lightAmbient; //直接得出环境光的最终强度
    vec3 normalTarget=aPosition+normal; //计算变换后的法向量
    vec3 newNormal=(uMMatrix*vec4(normalTarget,1)).xyz-(uMMatrix*vec4(aPosition,1)).xyz;
    newNormal=normalize(newNormal); //对法向量规格化
    vec3 eye= normalize(uCamera-(uMMatrix*vec4(aPosition,1)).xyz); //被照射顶点到摄像机的向量
    vec3 vp= normalize(lightDirection); //格式化定向光方向向量
    vec3 halfVector=normalize(vp+eye); //求视线与光线向量的半向量
    float shininess=50.0; //粗糙度，越小越光滑
    float nDotViewPosition=max(0.0,dot(newNormal,vp)); //求法向量与vp 的点积与0 的最大值
    diffuse=lightDiffuse*nDotViewPosition; //计算散射光的最终强度
    float nDotViewHalfVector=dot(newNormal,halfVector); //法线与半向量的点积
    float powerFactor=max(0.0,pow(nDotViewHalfVector,shininess)); //镜面反射光强度因子
    specular=lightSpecular*powerFactor; //计算镜面光的最终强度
    return ambient+diffuse+specular; //将三个光照通道最终强度值求和返回
}
void main(){
    outLightQD=directionalLight ( //将最终光照强度传递给片元着色器
                            myConstantVals.mm, //基本变换矩阵
                            myBufferVals.uCamera.xyz, //摄像机位置
                            myBufferVals. uLightDirection.xyz, //定向光方向
                            myBufferVals.lightAmbient, //环境光强度
                            myBufferVals.lightDiffuse, //散射光强度
                            myBufferVals.lightSpecular, //镜面光强度
                            inNormal, //法向量
                            pos //顶点位置
                            );
    gl_Position = myConstantVals.mvp * vec4(pos,1.0); //计算顶点最终位置
    vposition=pos; //传递顶点位置给片元着色器
}
