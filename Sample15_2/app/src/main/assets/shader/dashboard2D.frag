#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals {//一致变量块
    mat4 uViewProjectionInverseMatrix;//当前帧的投影与观察组合矩阵的逆矩阵
    mat4 uPreviousProjectionMatrix;//前一帧的投影与观察组合矩阵
    float g_numSamples;//运动模糊采样数
} myBufferVals;
layout (binding = 1) uniform sampler2D Tex;//纹理采样器（颜色）
layout (binding = 2) uniform sampler2D depthTex;//纹理采样器（深度）
layout (location = 0) in vec2 inTexCoor;//接收从顶点着色器过来的纹理坐标
layout (location = 0) out vec4 outColor;//输出到管线的片元颜色值
void main() {
      vec2 textureCoord=inTexCoor;//拷贝纹理坐标
      float zOverW = texture(depthTex,textureCoord).r;//采样得到此片元的深度值
      vec4 H = vec4((textureCoord.x*2.0-1.0),textureCoord.y*2.0-1.0,zOverW,1.0);//求片元的标准设备空间坐标H(三轴范围皆为-1~1)
      vec4 D = myBufferVals.uViewProjectionInverseMatrix*H;//通过投影与观察组合矩阵的逆阵进行变换
      vec4 worldPos= D/D.w;//执行透视除法，得到世界空间中的坐标
      vec4 currentPos=H;//片元在标准设备空间中的坐标(当前帧)
      vec4 previousPos=myBufferVals.uPreviousProjectionMatrix*worldPos;//使用世界坐标系坐标，并通过前一帧投影与观察组合矩阵进行变换
      previousPos=previousPos/previousPos.w;//执行透视除法得到前一帧标准设备空间中的坐标
      vec2 velocity=((previousPos-currentPos)/myBufferVals.g_numSamples).xy;//基于当前帧和前一帧标准设备空间坐标及采样数来计算当前片元的模糊方向步进向量
      vec4 color=texture(Tex, textureCoord);//采样得到此片元位置的颜色值
      textureCoord+=velocity;//通过模糊方向步进向量扰动纹理坐标
      for(int i=1;i<myBufferVals.g_numSamples;i++,textureCoord+=velocity)//循环指定次数
      {
            vec4 currentColor=texture(Tex, textureCoord);//根据扰动后的纹理坐标进行采样
            color+=currentColor;//将当前颜色累加到颜色和中
      }
      outColor=color/myBufferVals.g_numSamples;//对采样叠加的结果求平均，得到最终的颜色值
}