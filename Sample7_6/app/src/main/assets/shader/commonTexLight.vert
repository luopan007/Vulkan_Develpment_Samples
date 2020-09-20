#version 400
#extension GL_ARB_separate_shader_objects:enable //开启GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack:enable //开启GL_ARB_shading_language_420pack
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致变量块
vec4 uCamera; //摄像机位置
vec4 lightPosition; //光源位置
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
layout (location = 0) out vec4 outLightQD; //输出到片元着色器的正面光照强度
layout (location = 1) out vec4 outLightQDBack; //输出到片元着色器的反面光照强度
out gl_PerVertex { //输出接口块
vec4 gl_Position; //内建变量gl_Position
};

vec4 pointLight( 
    in mat4 uMMatrix, 
    in vec3 uCamera, 
    in vec3 lightLocation, 
    in vec4 lightAmbient, 
    in vec4 lightDiffuse, 
    in vec4 lightSpecular, 
    in vec3 normal, 
    in vec3 aPosition 
){
    vec4 ambient; 
    vec4 diffuse; 
    vec4 specular; 
    ambient=lightAmbient; 
    vec3 normalTarget=aPosition+normal; 
    vec3 newNormal=(uMMatrix*vec4(normalTarget,1)).xyz-(uMMatrix*vec4(aPosition,1)).xyz;
    newNormal=normalize(newNormal); 
    vec3 eye= normalize(uCamera-(uMMatrix*vec4(aPosition,1)).xyz); 
    vec3 vp= normalize(lightLocation-(uMMatrix*vec4(aPosition,1)).xyz);
    vp=normalize(vp); 
    vec3 halfVector=normalize(vp+eye); 
    float shininess=50.0; 
    float nDotViewPosition=max(0.0,dot(newNormal,vp)); 
    diffuse=lightDiffuse*nDotViewPosition; 
    float nDotViewHalfVector=dot(newNormal,halfVector); 
    float powerFactor=max(0.0,pow(nDotViewHalfVector,shininess)); 
    specular=lightSpecular*powerFactor; 
    return ambient+diffuse+specular; 
}
void main(){
    outLightQD=pointLight( 
                            myConstantVals.mm, 
                            myBufferVals.uCamera.xyz, 
                            myBufferVals.lightPosition.xyz, 
                            myBufferVals.lightAmbient, 
                            myBufferVals.lightDiffuse, 
                            myBufferVals.lightSpecular, 
                            inNormal, 
                            pos 
                            );
    outLightQDBack=pointLight( //计算反面的光照强度
                            myConstantVals.mm, //基本变换矩阵
                            myBufferVals.uCamera.xyz, //摄像机位置
                            myBufferVals.lightPosition.xyz, //光源位置
                            myBufferVals.lightAmbient, //环境光强度
                            myBufferVals.lightDiffuse, //散射光强度
                            myBufferVals.lightSpecular, //镜面光强度
                            -inNormal, //法向量（反面的）
                            pos //顶点位置
                            );
    gl_Position = myConstantVals.mvp * vec4(pos,1.0); //计算顶点最终位置
}
