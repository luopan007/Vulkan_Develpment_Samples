#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

//目前的实验其表明 必须是4的整数倍 否则奇怪现象
layout (std140,set = 0, binding = 0) uniform bufferVals {
    vec4 uCamera;
	vec4 lightPosition;
	vec4 lightAmbient;
	vec4 lightDiffuse;
	vec4 lightSpecular;
} myBufferVals;

layout (push_constant) uniform constantVals {
	mat4 mvp;
	mat4 mm;
} myConstantVals;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTexCoor;
layout (location = 2) in vec3 inNormal;
layout (location = 0) out vec2 outTexCoor;
layout (location = 1) out vec4 outLightQD;
out gl_PerVertex {
	vec4 gl_Position;
};

vec4 pointLight(				//定位光光照计算的方法
  in mat4 uMMatrix,             //基本变换矩阵
  in vec3 uCamera,              //摄像机位置
  in vec3 lightLocation,		//光源位置
  in vec4 lightAmbient,			//环境光强度
  in vec4 lightDiffuse,			//散射光强度
  in vec4 lightSpecular,		//镜面光强度
  in vec3 normal,				//法向量
  in vec3 aPosition             //顶点位置
){
  vec4 ambient;			//环境光最终强度
  vec4 diffuse;			//散射光最终强度
  vec4 specular;		//镜面光最终强度

  ambient=lightAmbient;			//直接得出环境光的最终强度
  vec3 normalTarget=aPosition+normal;	//计算变换后的法向量
  vec3 newNormal=(uMMatrix*vec4(normalTarget,1)).xyz-(uMMatrix*vec4(aPosition,1)).xyz;
  newNormal=normalize(newNormal); 	//对法向量规格化
  //计算从表面点到摄像机的向量
  vec3 eye= normalize(uCamera-(uMMatrix*vec4(aPosition,1)).xyz);
  //计算从表面点到光源位置的向量vp
  vec3 vp= normalize(lightLocation-(uMMatrix*vec4(aPosition,1)).xyz);
  vp=normalize(vp);//格式化vp
  vec3 halfVector=normalize(vp+eye);	//求视线与光线的半向量
  float shininess=50.0;				//粗糙度，越小越光滑
  float nDotViewPosition=max(0.0,dot(newNormal,vp)); 	//求法向量与vp的点积与0的最大值
  diffuse=lightDiffuse*nDotViewPosition;				//计算散射光的最终强度
  float nDotViewHalfVector=dot(newNormal,halfVector);	//法线与半向量的点积
  float powerFactor=max(0.0,pow(nDotViewHalfVector,shininess)); 	//镜面反射光强度因子
  specular=lightSpecular*powerFactor;    			//计算镜面光的最终强度
  //返回结果
  return ambient+diffuse+specular;
}

void main() {

    outTexCoor = inTexCoor;
   outLightQD=pointLight(				//定位光光照计算的方法
                                myConstantVals.mm,             //基本变换矩阵
                                myBufferVals.uCamera.xyz,              //摄像机位置
                                myBufferVals.lightPosition.xyz,		//光源位置
                                myBufferVals.lightAmbient,			//环境光强度
                                myBufferVals.lightDiffuse,			//散射光强度
                                myBufferVals.lightSpecular,		//镜面光强度
                                inNormal,				//法向量
                                pos            //顶点位置
                              );


    gl_Position = myConstantVals.mvp * vec4(pos,1.0);
}
