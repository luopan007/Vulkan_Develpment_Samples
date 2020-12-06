#version 400
#extension GL_ARB_separate_shader_objects:enable //开启GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack:enable //开启GL_ARB_shading_language_420pack
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致变量块
vec4 uCamera; //摄像机位置
vec4 lightPosition; //光源位置
vec4 lightAmbient; //环境光强度
vec4 lightDiffuse; //散射光强度
vec4 lightSpecular; //镜面光强度
vec4 u_clipPlane; //剪裁平面4 参数
} myBufferVals;
layout (push_constant) uniform constantVals{
    mat4 mvp;
    mat4 mm;
} myConstantVals;
layout (location = 0) in vec3 pos; 
layout (location = 1) in vec3 inNormal; 
layout (location = 0) out vec4 outLightQD;
layout (location = 1) out float u_clipDist; //输出到片元着色器的剪裁数据
out gl_PerVertex { //输出接口块
vec4 gl_Position; //内建变量gl_Position
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
void main(){//主方法
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
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);//计算顶点最终位置
    //将顶点位置(x0,y0,z0)代入平面方程表达式Ax+By+Cz+D,并将计算结果传递给片元着色器
       u_clipDist = dot(pos.xyz, myBufferVals.u_clipPlane.xyz) +myBufferVals.u_clipPlane.w;
}
