#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致变量块
    vec4 uCamera; //摄像机位置
    vec4 lightPosition; //光源位置
    vec4 lightDiffuse; //散射光强度
    vec4 lightSpecular; //镜面光强度
    mat4 mpc; //投影与观察组合矩阵
} myBufferVals;
layout (push_constant) uniform constantVals { //推送常量块
mat4 mvp; //总变换矩阵
mat4 mm; //基本变换矩阵
float isShadow; //阴影绘制标志
} myConstantVals;
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 2) in vec3 inNormal; //输入的法向量
layout (location = 0) out vec4 outLightQD; //输出的光照强度
layout (location = 1) out vec2 outTexCoor; //输出的纹理坐标
layout (location = 2) out float shadow; //输出的阴影绘制标志
out gl_PerVertex {vec4 gl_Position; }; //内建变量 gl_Position
vec4 pointLight(				//定位光光照计算的方法
  in mat4 uMMatrix,             //基本变换矩阵
  in vec3 uCamera,              //摄像机位置
  in vec3 lightLocation,		//光源位置
  in vec4 lightDiffuse,			//散射光强度
  in vec4 lightSpecular,		//镜面光强度
  in vec3 normal,				//法向量
  in vec3 aPosition             //顶点位置
){
  vec4 diffuse;			//散射光最终强度
  vec4 specular;		//镜面光最终强度
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
  return diffuse+specular;
}
void main() {
   outLightQD=pointLight(				//定位光光照计算的方法
                                myConstantVals.mm,             //基本变换矩阵
                                myBufferVals.uCamera.xyz,              //摄像机位置
                                myBufferVals.lightPosition.xyz,		//光源位置
                                myBufferVals.lightDiffuse,			//散射光强度
                                myBufferVals.lightSpecular,		//镜面光强度
                                inNormal,				//法向量
                                pos            //顶点位置
                              );
   if(myConstantVals.isShadow==1){						//标志位为1，则绘制阴影
      vec3 A=vec3(0.0,0.1,0.0);			//绘制阴影平面上任意一点的坐标
      vec3 n=vec3(0.0,1.0,0.0);			//绘制阴影平面的法向量
      vec3 S=myBufferVals.lightPosition.xyz; 				//光源位置
      vec3 V=(myConstantVals.mm*vec4(pos,1)).xyz;  		//经过平移和旋转变换后的点的坐标
      vec3 VL=S+(V-S)*(dot(n,(A-S))/dot(n,(V-S)));//顶点沿光线投影到需要绘制阴影的平面上点的坐标
      gl_Position = myBufferVals.mpc*vec4(VL,1); 	//根据组合矩阵计算此次绘制此顶点位置
   }else{   							//根据总变换矩阵计算此次绘制此顶点位置
      gl_Position = myConstantVals.mvp * vec4(pos,1);
   }
    outTexCoor = inTexCoor; //将接收的纹理坐标传递给片元着色器
    shadow=myConstantVals.isShadow; //将接收的阴影绘制标志传递给片元着色器
}
