#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 vcolor;
layout (location = 1) in vec3 vPosition;
layout (location = 0) out vec4 outColor;

void main(){
   vec4 finalColor=vec4(vcolor.rgb,0.0);
      //绕z轴转20度的旋转变换矩阵
      mat4 mm=mat4
      ( 0.9396926,-0.34202012,0.0,0.0,
        0.34202012,0.9396926,0.0,0.0,
        0.0,0.0,1.0,0.0,
        0.0,0.0,0.0,1.0);
      vec4 tPosition=mm*vec4(vPosition,1);//将顶点坐标绕z轴转20度
      if(mod(tPosition.x+50.0,8)>6) {//计算X方向在不在红光色带范围内
        finalColor=vec4(0.4,0.0,0.0,1.0)+finalColor;//若在给最终颜色加上淡红色
      }
   outColor=finalColor;//给此片元颜色值
}
