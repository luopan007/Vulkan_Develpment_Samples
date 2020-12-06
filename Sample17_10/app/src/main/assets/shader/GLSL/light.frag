#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (input_attachment_index = 0, binding = 0) //输入附件0(存储各片元的颜色值)
uniform subpassInput colorUni;
layout (input_attachment_index = 1, binding = 1) //输入附件1(存储各片元世界坐标系中的位置坐标)
uniform subpassInput positionUni;
layout (input_attachment_index = 2, binding = 2) //输入附件2(存储各片元世界坐标系中的法向量)
uniform subpassInput normalUni;
layout(constant_id=0) const int LIGHTS_COUNT=4;
struct Light
{
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};
layout (std140,set = 0, binding = 3) uniform bufferVals {
    vec4 uCamera;						
	Light lightArray[LIGHTS_COUNT];		
} myBufferVals;
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
void main() 
{
	vec3 postion = subpassLoad(positionUni).rgb; //从输入附件中提取片元在世界坐标系中的位置坐标
	vec3 normal = subpassLoad(normalUni).rgb; //从输入附件中提取片元在世界坐标系中的法向量
	vec4 color = subpassLoad(colorUni); //从输入附件中提取片元的颜色值
	vec4 lightQD=vec4(0.0);
	for(int i=0;i<LIGHTS_COUNT;i++)
	{
		lightQD+=pointLight(				
                                myBufferVals.uCamera.xyz,             
                                myBufferVals.lightArray[i].position,		
                                myBufferVals.lightArray[i].ambient,			
                                myBufferVals.lightArray[i].diffuse,			
                                myBufferVals.lightArray[i].specular,		
                                normal,				
                                postion            
                              );
	}
	lightQD.a=1.0;
    outColor = color*lightQD; //产生最终片元颜色值
}