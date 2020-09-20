#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致变量块
vec4 uCamera; //摄像机位置
vec4 lightPosition; //光源位置
float lightDiffuse; //散射光强度
float lightSpecular; //镜面光强度
} myBufferVals;
layout (push_constant) uniform constantVals { //推送常量块
mat4 mvp; //最终变换矩阵
mat4 mm; //基本变换矩阵
} myConstantVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 2) in vec3 inNormal; //输入的法向量
layout (location = 0) out float vEdge; //输出到片元着色器的描边系数
layout (location = 1) out vec2 outTexCoor; //输出到片元着色器的纹理坐标
out gl_PerVertex { //输出接口块
vec4 gl_Position; //内建变量gl_Position
};
float pointLight(
  in mat4 uMMatrix,
  in vec3 uCamera,
  in vec3 lightLocation,
  in float lightDiffuse,
  in float lightSpecular,
  in vec3 normal,
  in vec3 aPosition
){
  float diffuse;
  float specular;
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
  return diffuse+specular;
}
float fun( //计算描边系数的方法
in mat4 uMMatrix, //基本变换矩阵
in vec3 uCamera, //摄像机位置
in vec3 aPosition, //顶点位置
in vec3 normal){ //顶点法向量
vec3 newNormal=normalize((uMMatrix*vec4(normal,0)).xyz); //将法向量变换到世界坐标系中
vec3 eye= normalize(uCamera-(uMMatrix*vec4(aPosition,1)).xyz);//计算从表面点到摄像机的向量
return max(0.0,dot(newNormal,eye)); //计算描边系数
}
void main() {
   gl_Position = myConstantVals.mvp * vec4(pos,1.0);//计算顶点的最终位置
   float LightQD=pointLight(	//进行光照计算
                                myConstantVals.mm,
                                myBufferVals.uCamera.xyz,
                                myBufferVals.lightPosition.xyz,
                                myBufferVals.lightDiffuse,
                                myBufferVals.lightSpecular,
                                inNormal,
                                pos
                              );
	vEdge = fun(myConstantVals.mm, //计算描边系数并传给片元着色器
	myBufferVals.uCamera.xyz,pos,inNormal);
	outTexCoor = vec2(LightQD,0.5); //根据光照强度折算纹理坐标
}
