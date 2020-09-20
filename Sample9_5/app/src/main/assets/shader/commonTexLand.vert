#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals { //推送常量块
mat4 mvp; //最终变换矩阵
float landHighAdjust; //山地高度调整值
float landHighest; //山地最大高差
} myConstantVals;
layout (binding = 3) uniform sampler2D texLand; //灰度图纹理
layout (location = 0) in vec3 pos; //输入的顶点位置
layout (location = 1) in vec2 inTexCoor; //输入的纹理坐标(外观)
layout (location = 2) in vec2 inLandTex; //输入的纹理坐标(高度)
layout (location = 0) out vec2 outTexCoor; //输出到片元着色器的纹理坐标(外观)
layout (location = 1) out float landHeight; //输出到片元着色器的Y 坐标
out gl_PerVertex { //输出接口块
vec4 gl_Position; //内建变量gl_Position
};
void main() {
outTexCoor = inTexCoor; //将接收到的纹理坐标传递到片元着色器
vec4 gColor = textureLod(texLand, inLandTex, 0.0); //从灰度图中采样出颜色值
float tempY = (((gColor.r + gColor.g + gColor.b) / 3.0) //通过灰度图颜色值计算出顶点Y 坐标
* myConstantVals.landHighest)+myConstantVals.landHighAdjust;
landHeight = tempY; //将顶点Y 坐标传递到片元着色器
gl_Position = myConstantVals.mvp * vec4(pos.x,tempY,pos.z, 1);//计算顶点最终位置
}