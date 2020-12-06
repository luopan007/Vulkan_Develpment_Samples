#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals {
	mat4 vp;
} myConstantVals;
layout (binding = 2) uniform sampler2D sNoise;
layout (std140,set = 0, binding = 0) uniform bufferVals {
    vec4 uCamera;
	vec4 lightPosition;
	vec4 lightAmbient;
	vec4 lightDiffuse;
	vec4 lightSpecular;
	float personAngle; //小星体自转角
    float rotateAngle; //小星体公转角
    float groupNumber; //每组星体的数量
    float oneGroupNumber; //星体组数
    float distanceDai; //土星光环最内侧与土星的距离
    float linWidthSpan; //组内各个小星体之间的距离步进
    float angleSpan; //每组小星体之间的角度跨度
} myBufferVals;
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoor;
layout (location = 2) in vec3 inNormal;
layout (location = 0) out vec2 vTextureCoord;
layout (location = 1) out vec4 outLightQD;
out gl_PerVertex {
	vec4 gl_Position;
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
void main() {
   int groupNumber=int(myBufferVals.groupNumber); //获得小星体组数
   int oneGroupNumber=int(myBufferVals.oneGroupNumber); //获得每组小星体的数量
   float distanceDai=myBufferVals.distanceDai; //获得光环内侧与土星的距离
   float linWidthSpan=myBufferVals.linWidthSpan; //获得组内小星体之间的距离步进
   float angleSpan=myBufferVals.angleSpan; //获得每组小星体之间的角度跨度
   int groupID=gl_InstanceIndex/oneGroupNumber; //计算当前实例所属组编号
   int inGroupID=gl_InstanceIndex%oneGroupNumber; //计算当前实例组内编号
   float xtex=float(groupID)/float(groupNumber); //计算扰动纹理中的S 坐标
   float ytex=float(inGroupID)/float(oneGroupNumber); //计算扰动纹理中的T 坐标
   vec4 noiseVec=texture(sNoise,vec2(xtex,ytex)); //进行扰动纹理采样
   float random=noiseVec.r*10.0; //获得0.0~10.0 之间的伪随机数
   float translateDis=distanceDai+linWidthSpan*inGroupID; //计算小星体距土星中心的距离
   float curAngle=(groupID-1)*angleSpan; //计算当前小星体的公转角
   float stoneAngle=curAngle+myBufferVals.rotateAngle+angleSpan*random; //扰动小星体公转角
   float translateX=translateDis*cos(radians(stoneAngle)); //计算小星体X 坐标
   float translateZ=translateDis*sin(radians(stoneAngle)); //计算小星体Z 坐标
   mat4 translateM=mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, translateX,0,translateZ,1);//推导小星体平移矩阵
   float perAngle=myBufferVals.personAngle; //获得当前小星体自转角
   mat4 rotateM=mat4(1,0,0,0, 0,cos(radians(perAngle)), //推导小星体旋转矩阵
   sin(radians(perAngle)),0, 0,-sin(radians(perAngle)),cos(radians(perAngle)),0, 0,0,0,1);
   mat4 mm=translateM*rotateM; //计算基本变换矩阵
   mat4 mvp=myConstantVals.vp*mm; //计算最终变换矩阵
   gl_Position =mvp*vec4(aPosition,1.0); //计算最终顶点位置
   vTextureCoord = aTexCoor; //将纹理坐标传递给片元着色器
    outLightQD=pointLight(//定位光光照计算的方法
                                        mm,                                 //基本变换矩阵
                                        myBufferVals.uCamera.xyz,           //摄像机位置
                                        myBufferVals.lightPosition.xyz,		//光源位置
                                        myBufferVals.lightAmbient,			//环境光强度
                                        myBufferVals.lightDiffuse,			//散射光强度
                                        myBufferVals.lightSpecular,		    //镜面光强度
                                        inNormal,				            //法向量
                                        aPosition                                 //顶点位置
        );
}
