#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 0) uniform sampler2D colorUni; //存储各片元颜色值的纹理
layout (binding = 1) uniform sampler2D positionUni; //存储各片元在世界坐标系中位置坐标的纹理
layout (binding = 2) uniform sampler2D normalUni; //存储各片元在世界坐标系中法向量的纹理
layout(constant_id=0) const int LIGHTS_COUNT=4; //光源数量
struct Light{ //光源数据结构体
  vec4 position; //光源位置
  vec4 ambient; //光源环境光参数
  vec4 diffuse; //光源散射光参数
  vec4 specular; //光源镜面光参数
};
layout (std140,set = 0, binding = 3) uniform bufferVals { //一致变量块
vec4 uCamera; //摄像机位置
Light lightArray[LIGHTS_COUNT]; //各个光源的参数数组
} myBufferVals;
layout (location = 0) in vec2 inTexCoor; //片元的纹理坐标
layout (location = 0) out vec4 outColor; //最终输出用的片元颜色值（有光照）
vec4 pointLight(in vec3 uCamera,in vec4 lightLocation,in vec4 lightAmbient, //定位光光照计算的方法
in vec4 lightDiffuse,in vec4 lightSpecular,in vec3 normal,in vec3 aPosition){
vec4 ambient; //环境光强度
vec4 diffuse; //散射光强度
vec4 specular; //镜面光强度
ambient=lightAmbient; //直接得出环境光的最终强度
float dis=distance(lightLocation.xyz,aPosition); //计算到光源的距离
if(dis>lightLocation.w){ //如果片元到光源的位置大于光源的范围
return ambient; //直接返回环境光
}
  vec3 eye= normalize(uCamera-aPosition);
  vec3 vp= normalize(lightLocation.xyz-aPosition);
  vp=normalize(vp);
  vec3 halfVector=normalize(vp+eye);	
  float shininess=50.0;				
  float nDotViewPosition=max(0.0,dot(normal,vp)); 	
  diffuse=lightDiffuse*nDotViewPosition;				
  float nDotViewHalfVector=dot(normal,halfVector);	
  float powerFactor=max(0.0,pow(nDotViewHalfVector,shininess)); 	
  specular=lightSpecular*powerFactor;    			
  return ambient+diffuse+specular;
}
void main(){ //主方法
vec3 postion = texture (positionUni,
inTexCoor).rgb; //从纹理中提取当前处理片元在世界坐标系中的位置坐标
vec3 normal = texture (normalUni,
inTexCoor).rgb; //从纹理中提取当前处理片元在世界坐标系中的法向量
vec4 color = texture (colorUni, inTexCoor); //从纹理中提取当前处理片元的无光照颜色值
vec4 lightQD=vec4(0.0); //所有光源定位光光照计算的总结果
for(int i=0;i<LIGHTS_COUNT;i++){ //遍历所有光源
lightQD+=pointLight(myBufferVals.uCamera.xyz, //执行定位光光照计算
myBufferVals.lightArray[i].position,myBufferVals.lightArray[i].ambient,
myBufferVals.lightArray[i].diffuse,myBufferVals.lightArray[i].specular,normal,postion);
}
lightQD.a=1.0; //设置片元完全不透明
outColor = lightQD*color; //产生最终片元颜色值（有光照）
}