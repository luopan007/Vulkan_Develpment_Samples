#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec4 inLightQD;//顶点着色器传入的最终光照强度
layout (location = 1) in vec2 mcLongLat;//顶点着色器传入的顶点经纬度
layout (location = 0) out vec4 outColor;//输出到渲染管线的片元颜色值
void main() {
      vec3 color;//当前片元的颜色值
      if(abs(mcLongLat.y)>75.0){
      	  color = vec3(1.0,1.0,1.0);//两极是白色
      }else{
         int colorNum = int(mcLongLat.x/45.0);//颜色号
         vec3 colorArray[8]=vec3[8](//颜色数组
         vec3(1.0,0.0,0.0),vec3(0.0,1.0,0.0),vec3(0.0,0.0,1.0),
         vec3(1.0,1.0,0.0),vec3(1.0,0.0,1.0),vec3(0.0,1.0,1.0),
         vec3(0.3,0.4,0.7),vec3(0.3,0.7,0.2));
    		color=colorArray[colorNum];//根据索引获得颜色值
	}
    outColor = vec4(color,1.0)*inLightQD;//结合光照强度产生片元最终颜色值
}