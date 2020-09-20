#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec4 inLightQD;
layout (location = 1) in vec3 vposition;
layout (location = 0) out vec4 outColor;

vec4 genBoardColor(vec3 position)
{
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

void main() {
   outColor=inLightQD*genBoardColor(vposition);
}