#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致变量块
float brightFactor; //亮度调节系数
float landStartY; //过程纹理起始Y 坐标
float landYSpan; //过程纹理跨度
} myBufferVals;
layout (binding = 1) uniform sampler2D texGrass; //纹理数据（草地）
layout (binding = 2) uniform sampler2D texRock; //纹理数据（岩石）
layout (location = 0) in vec2 inTexCoor; //接收到的纹理坐标
layout (location = 1) in float landHeight; //接收到的Y 坐标
layout (location = 0) out vec4 outColor; //输出到管线的片元颜色
void main() {
vec4 grassColor = textureLod(texGrass, inTexCoor, 0.0); //从草地纹理中采样出颜色
vec4 rockColor = textureLod(texRock, inTexCoor, 0.0); //从岩石纹理中采样出颜色
vec4 finalColor; //最终的颜色
if(landHeight < myBufferVals.landStartY){
finalColor = grassColor; //当片元Y 坐标小于过程纹理起始Y 坐标时采用草地纹理
}else if(landHeight > myBufferVals.landStartY + myBufferVals.landYSpan){
finalColor = rockColor; //当片元Y 坐标大于过程纹理起始Y 坐标加跨度值时采用岩石纹理
}else{ //当片元Y 坐标在起始Y 坐标及起始Y 坐标加跨度值之间时
float currYRatio=(landHeight-myBufferVals.landStartY)/myBufferVals.landYSpan;//计算岩石纹理所占比例
finalColor=currYRatio * rockColor+(1.0-currYRatio)*grassColor;//将岩石，草地纹理按比例混合
}
outColor=myBufferVals.brightFactor*finalColor; //此片元最终颜色值
}