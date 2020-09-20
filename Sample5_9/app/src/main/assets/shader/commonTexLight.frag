#version 450
#extension GL_ARB_separate_shader_objects : enable //打开GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable//打开GL_ARB_shading_language_420pack
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致块
vec4 uCamera; //摄像机位置
vec4 lightPosition; //光源位置
vec4 lightAmbient; //环境光强度
vec4 lightDiffuse; //散射光强度
vec4 lightSpecular; //镜面光强度
} myBufferVals;
layout (location = 1) in vec3 vposition; //传入的世界坐标系顶点位置
layout (location = 2) in vec3 vNormal; //传入的世界坐标系法向量
layout (location = 3) in vec3 objPos; //传入的物体坐标系顶点位置
layout (location = 0) out vec4 outColor; //输出到渲染管线的最终片元颜色
vec4 genBoardColor(vec3 position){
   const float height=4.0;//球的半径
   const float width=6.0;
   vec4 color;
   float n = 8.0;//外接立方体每个坐标轴方向切分的份数
   float spanh = height/n;//每一份的尺寸（小方块的边长）
   float spanw = width/n;
   //为了保证不出现负数
   int i = int((position.x + 10.0)/spanw);//当前片元位置小方块的行数
   int j = int((position.y + 10.0)/spanh);//当前片元位置小方块的层数

    //计算当前片元行数、层数、列数的和并对2取模
   int whichColor = int(mod(float(i+j),2.0));
   if(whichColor == 1) {//奇数时为红色
   		color = vec4(0.678,0.231,0.129,1.0);//红色
   }
   else {//偶数时为白色
   		color = vec4(1.0,1.0,1.0,1.0);//白色
   }
   return color;
}
vec4 pointLight( //定位光光照计算的方法
    in vec3 uCamera, //摄像机位置
    in vec3 lightLocation, //光源位置
    in vec4 lightAmbient, //环境光强度
    in vec4 lightDiffuse, //散射光强度
    in vec4 lightSpecular, //镜面光强度
    in vec3 normal, //法向量
    in vec3 aPosition){ //顶点位置
        vec4 ambient; //环境光最终强度
        vec4 diffuse; //散射光最终强度
        vec4 specular; //镜面光最终强度
        ambient=lightAmbient; //直接得出环境光的最终强度
        vec3 eye= normalize(uCamera-aPosition); //计算从表面点到摄像机的向量
        vec3 vp= normalize(lightLocation.xyz-aPosition); //计算从表面点到光源位置的向量vp
        vp=normalize(vp); //格式化vp 向量
        vec3 halfVector=normalize(vp+eye); //求视线与vp 向量的半向量
        float shininess=10.0; //粗糙度，越小越光滑
        float nDotViewPosition=max(0.0,dot(normal,vp)); //求法向量与vp 的点积与0 的最大值
        diffuse=lightDiffuse*nDotViewPosition; //计算散射光的最终强度
        float nDotViewHalfVector=dot(normal,halfVector); //法向量与半向量的点积
        float powerFactor=max(0.0,pow(nDotViewHalfVector,shininess)); //镜面反射光强度因子
        specular=lightSpecular*powerFactor; //计算镜面光的最终强度
        return ambient+diffuse+specular; //将三个光照通道最终强度值求和返回
}
void main() {
    vec4 lightQD=pointLight( //定位光光照计算的方法
                    myBufferVals.uCamera.xyz, //摄像机位置
                    myBufferVals.lightPosition.xyz, //光源位置
                    myBufferVals.lightAmbient, //环境光强度
                    myBufferVals.lightDiffuse, //散射光强度
                    myBufferVals.lightSpecular, //镜面光强度
                    vNormal, //世界坐标系法向量
                    vposition ); //世界坐标系顶点位置
    outColor=genBoardColor(objPos)*lightQD; //计算片元最终颜色值
}