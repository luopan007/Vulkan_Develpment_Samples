#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 0) uniform sampler2D colorUni;
layout (binding = 1) uniform sampler2D positionUni;
layout (binding = 2) uniform sampler2D normalUni;
layout(constant_id=0) const int LIGHTS_COUNT=4;
struct Light
{
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};
layout (binding = 3) uniform sampler2D aoFactorUni; //纹理采样器（片元的环境光遮挡因子）
layout (std140,set = 0, binding = 4) uniform bufferVals { //一致块
vec4 uCamera; //摄像机位置
Light lightArray[LIGHTS_COUNT]; //各个光源的参数
float ifSSAO; //环境光遮挡开启标志
} myBufferVals;
layout (location = 0) in vec2 inTexCoor;
layout (location = 0) out vec4 outColor;
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
float getAOFactor(vec2 texCoorIn){ //进行模糊处理得到当前片元的环境光遮挡因子值的方法
const float stStep = 1024.0; //纹理偏移量调整系数
const float scaleFactor = 1.0/9.0; //定义最终求和时的加权因子(为调整亮度)
vec2 offsets[9]=vec2[9]( //卷积内核中各个元素对应纹素相对于待处理纹素的纹理坐标偏移量
vec2(-1.0,-1.0),vec2(0.0,-1.0),vec2(1.0,-1.0),
vec2(-1.0,0.0),vec2(0.0,0.0),vec2(1.0,0.0),
vec2(-1.0,1.0),vec2(0.0,1.0),vec2(1.0,1.0)
);
float kernelValues[9]=float[9](1.0,1.0,1.0, 1.0,1.0,1.0, 1.0,1.0,1.0 ); //卷积内核中各个位置的权值
vec4 sum=vec4(0,0,0,0); //最终颜色值
for(int i=0;i<9;i++){ //执行卷积
sum=sum+kernelValues[i]*scaleFactor*texture(aoFactorUni, texCoorIn+offsets[i]/stStep);
}
return sum.r; //返回环境光遮挡因子
}
void main()
{
	vec4 color = texture(colorUni, inTexCoor);
	vec3 normal = texture(normalUni, inTexCoor).rgb;
	vec3 postion = texture(positionUni, inTexCoor).rgb;
	float aoFactor = getAOFactor(inTexCoor); //调用getAOFactor 方法获取当前片元的环境光遮挡因子
	vec4 lightQD=vec4(0.0);
	for(int i=0;i<LIGHTS_COUNT;i++)
	{
		lightQD+=pointLight(
                            myBufferVals.uCamera.xyz,             
                            myBufferVals.lightArray[i].position,		
                            myBufferVals.lightArray[i].ambient*(aoFactor*myBufferVals.ifSSAO+1.0*(1-myBufferVals.ifSSAO)),
                            myBufferVals.lightArray[i].diffuse,			
                            myBufferVals.lightArray[i].specular,		
                            normal,				
                            postion            
                          );
	}
	lightQD.a=1.0;
	outColor = lightQD*color;
}