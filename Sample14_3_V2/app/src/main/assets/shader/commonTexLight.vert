#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致变量块
    vec4 uCamera; //摄像机位置
    vec4 lightPosition; //光源位置
    vec4 lightAmbient; //环境光强度
    vec4 lightDiffuse; //散射光强度
    vec4 lightSpecular; //镜面光强度
} myBufferVals;
layout (push_constant) uniform constantVals { //推送常量块
    mat4 mvp; //总变换矩阵
    mat4 mm; //基本变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 2) in vec3 inNormal; //输入的法向量
layout (location = 0) out vec2 outTexCoor; //输出到片元着色器的纹理坐标
layout (location = 1) out vec4 outLightQD; //输出到片元着色器的光照强度
layout (location = 2) out vec4 vPosition; //输出到片元着色器的顶点坐标
layout (location = 3) out vec3 vNormal; //输出到片元着色器的法向量
layout (location = 4) out vec3 vlightPosition; //输出到片元着色器的光源位置
layout (location = 5) out vec4 vlightAmbient; //输出到片元着色器的环境光强度
out gl_PerVertex {vec4 gl_Position;}; //内建变量 gl_Position
vec4 pointLight(				//定位光光照计算的方法
  in mat4 uMMatrix,             //基本变换矩阵
  in vec3 uCamera,              //摄像机位置
  in vec3 lightLocation,		//光源位置
  in vec4 lightAmbient,			//环境光强度
  in vec4 lightDiffuse,			//散射光强度
  in vec4 lightSpecular,		//镜面光强度
  in vec3 normal,				//法向量
  in vec3 aPosition,             //顶点位置
  out vec3 newNormalOut            //变换后的法向量
){
  vec4 ambient;			//环境光最终强度
  vec4 diffuse;			//散射光最终强度
  vec4 specular;		//镜面光最终强度
  ambient=lightAmbient;			//直接得出环境光的最终强度
  vec3 normalTarget=aPosition+normal;	//计算变换后的法向量
  vec3 newNormal=(uMMatrix*vec4(normalTarget,1)).xyz-(uMMatrix*vec4(aPosition,1)).xyz;
  newNormal=normalize(newNormal); 	//对法向量规格化
  newNormalOut=newNormal;
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
                                pos,            //顶点位置
                                vNormal         //变换后的法向量
                              );
    vPosition=myConstantVals.mm*vec4(pos,1.0); //计算出变换后的顶点位置并传递给片元着色器
    vlightPosition=myBufferVals.lightPosition.xyz; //将接收的光源位置传递给片元着色器
    vlightAmbient=myBufferVals.lightAmbient; //将接收的环境光强度传递给片元着色器
    gl_Position = myConstantVals.mvp * vec4(pos,1.0); //根据总变换矩阵计算此次绘制此顶点位置
}


