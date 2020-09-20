#version 400
#extension GL_ARB_separate_shader_objects : enable //打开 GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable //打开 GL_ARB_shading_language_420pack
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致变量块
vec4 uCamera; //摄像机位置
vec4 lightPosition; //光源位置
vec4 lightAmbient; //环境光强度
vec4 lightDiffuse; //散射光强度
vec4 lightSpecular; //镜面光强度
} myBufferVals;
layout (push_constant) uniform constantVals { //推送常量块
mat4 mvp; //最终变换矩阵13
mat4 mm; //基本变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec3 inNormal; //输入的法向量
layout (location = 0) out vec4 outLightQD; //输出的光照强度
layout (location = 1) out float vFogFactor; //输出的雾化因子
out gl_PerVertex { //输出接口块
vec4 gl_Position; //内建变量 gl_Position
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

float computeFogFactor(){ //计算雾化因子的方法
    float tmpFactor; //存放雾化因子的变量
    float fogDistance = length(myBufferVals.uCamera.xyz -(myConstantVals.mm*vec4(pos,1)).xyz); //计算顶点到摄像机的距离
    const float end = 300.0; //雾结束的位置
    const float start = 200.0; //雾开始的位置
    tmpFactor = max(min((end- fogDistance)/(end-start),1.0),0.0); //计算雾化因子
    return tmpFactor; //返回雾化因子
}

void main() {
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

   gl_Position = myConstantVals.mvp * vec4(pos,1.0); //计算顶点最终位置
   vFogFactor = computeFogFactor(); //计算雾化因子值并传递给片元着色器
}
