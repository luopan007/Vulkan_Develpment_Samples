#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 0) uniform sampler2D colorUni; //纹理采样器（片元颜色）
layout (binding = 1) uniform sampler2D positionUni; //纹理采样器（片元在世界坐标系中的位置坐标）
layout (binding = 2) uniform sampler2D normalUni; //纹理采样器（片元在世界坐标系中的法向量）
layout(constant_id=0) const int LIGHTS_COUNT=4; //定义光源数量
struct Light{ //光源数据结构体
vec4 position; //光源位置
vec4 ambient; //环境光强度
vec4 diffuse; //散射光强度
vec4 specular;}; //镜面光强度
layout (std140,set = 0, binding = 3) uniform bufferVals { //一致变量块
vec4 uCamera; //摄像机位置
Light lightArray[LIGHTS_COUNT]; //各个光源的参数
mat4 mZHMatrix; //摄像机观察矩阵、投影矩阵与Vulkan 标准设备空间调整矩阵的组合矩阵
float ifSSAO; //是否启用环境光遮挡的标志
} myBufferVals;
layout (location = 0) in vec2 inTexCoor; //片元的纹理坐标
layout (location = 0) out vec4 outColor; //片元最终颜色值
vec4 pointLight(				
  in vec3 uCamera,              
  in vec4 lightLocation,		
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
  float dis=distance(lightLocation.xyz,aPosition);
  if(dis>lightLocation.w)
  {
    return ambient;
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
const int PCount=16; //考察点数量
const vec3[PCount] points={ //存储考察点坐标数据的数组
vec3(-1.4109830789736655,0.41551981760050594,1.3551642085117352),//第1 个考察点的位置坐标
vec3(-1.71563098357675,0.12153334149424075,-1.0207056260728247),
vec3(-1.5101061431210312,1.0916808145093917,0.7265070101108699), vec3(1.9069579480132612,-0.21607449824086555,0.5628704964012681), 
vec3(-1.0342702341865935,0.8133429356961823,1.5062398054854098), vec3(-1.3095804004461502,0.26983446269612166,-1.487342777408354), 
vec3(1.236515088080815,-1.477610154558723,-0.5363754916972261), vec3(1.4635171065477042,-1.23998466338527,0.5661763977874343), 
vec3(0.8394943992168215,0.10181501366699974,1.8124245795827139), vec3(1.5726076476390185,-0.4356637412915275,-1.1563313932913615), 
vec3(0.6378995218859248,-1.729368315533456,-0.7761246222139109), vec3(0.9945307789956596,-1.3632506714580492,1.0735250981693278), 
vec3(-0.19426588792219437,0.33826635898908297,1.961590333164922), vec3(-0.5288288092531159,-1.8756918542426992,0.449577755712532), 
vec3(0.3471964715180122,1.4894895198787435,-1.2887496189473082), vec3(-1.9821472826745812,0.03471594417648212,0.2643614060442807)
};
vec2 fromWorldToST(vec3 posIn,mat4 VP){ //将世界坐标系中的位置坐标转换为投影贴图纹理坐标
vec4 jckjPosition=VP*vec4(posIn.xyz,1.0); //将片元位置通过矩阵变换进剪裁空间中
jckjPosition=jckjPosition/jckjPosition.w; //执行透视除法将片元位置变换进标准设备空间中
float s=(jckjPosition.x+1.0)/2.0; //将标准设备空间中XOY 平面内的X 坐标变换为纹理S 坐标
float t=(jckjPosition.y+1.0)/2.0; //将标准设备空间中XOY 平面内的Y 坐标变换为纹理T 坐标
return vec2(s,t); //返回转换后的纹理坐标
}
void main(){ //主函数
vec4 color = texture(colorUni, inTexCoor); //从纹理中提取当前片元的颜色值
vec3 normal=texture(normalUni,inTexCoor).rgb; //提取当前片元在世界坐标系中的法向量
vec3 postion=texture(positionUni,inTexCoor).rgb; //提取当前片元在世界坐标系中的坐标
float AOCount=0.0; //位于物体内部的考察点数量
for(int i=0;i<PCount;i++){ //遍历所有考察点
vec3 posTemp=postion+points[i]; //将考察点转换到当前片元位置周围
float LTemp=distance(myBufferVals.uCamera.xyz,posTemp); //求考察点到摄像机的距离
vec2 stTemp=fromWorldToST(posTemp,myBufferVals.mZHMatrix);//将考察点坐标转为纹理坐标
vec3 posTempL=texture(positionUni,stTemp).rgb; //得到对应片元在世界坐标系中的坐标
float LTempL=distance(myBufferVals.uCamera.xyz,posTempL); //计算对应片元到摄像机的距离
if(LTempL<LTemp){ //若对应片元到摄像机的距离小于考察点到摄像机的距离
AOCount=AOCount+1.0; //位于物体内部的考察点数量加1
}}
float aoFactor=1.0-AOCount/float(PCount); //计算得到环境光遮挡因子
aoFactor=pow(aoFactor, 2.0); //对遮挡因子进行平方(仅仅为了效果)
vec4 lightQD=vec4(0.0); //声明表示最终光照强度的变量
for(int i=0;i<LIGHTS_COUNT;i++){ //遍历所有光源
lightQD+=pointLight( //调用计算光照的方法
myBufferVals.uCamera.xyz, //摄像机位置
myBufferVals.lightArray[i].position, //光源位置
myBufferVals.lightArray[i].ambient* //结合环境光遮挡因子和标志调整环境光强度
(aoFactor*myBufferVals.ifSSAO+1.0*(1-myBufferVals.ifSSAO)),
myBufferVals.lightArray[i].diffuse, //散射光强度
myBufferVals.lightArray[i].specular, //镜面光强度
normal, //法向量
postion //位置坐标
);}
lightQD.a=1.0; //设置最终光照强度的Alpha 通道
outColor = lightQD*color; //结合片元色彩得到最终片元颜色
}