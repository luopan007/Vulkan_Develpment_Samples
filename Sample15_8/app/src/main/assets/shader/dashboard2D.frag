#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals {//推送常量块
   layout(offset=64) float startX;//X 轴扫描起点
   float endX;//X 轴扫描终点
   float startY;//Y 轴扫描起点
   float endY;//Y 轴扫描终点
   float startZ;//Z 轴扫描起点
   float endZ;//Z 轴扫描终点
   float detailDepth;//透视深度控制系数
   float LFactor;//密度值范围调整系数
} myConstantValsFrag;
layout (binding = 0) uniform sampler2D texNSZ;//存储逆时针绘制各片元3D 纹理坐标的纹理
layout (binding = 1) uniform sampler2D texSSZ;//存储顺时针绘制各片元3D 纹理坐标的纹理
layout (binding = 2) uniform sampler3D tex3DVolume; //3D 纹理采样器（头部密度分布）
layout (binding = 3) uniform sampler2D tex2DPalette; //2D 纹理采样器 (调色板)
layout (location = 0) in vec2 inTexCoor;//接收从顶点着色器过来的纹理坐标
layout (location = 0) out vec4 outColor;//输出到管线的片元颜色
void main() {//主方法
   vec3 start=texture(texNSZ, inTexCoor).rgb;//获取起点(正面交点)3D 纹理坐标
   vec3 end=texture(texSSZ, inTexCoor).rgb;//获取终点(反面交点)3D 纹理坐标
   const float STEP=0.01;//纹理坐标步进(标量)
   float disFB=distance(start,end);//起点到终点的3D 纹理坐标距离
   int count=int(disFB/STEP);//纹理坐标总步数
   vec3 stepV=normalize(end-start)*STEP;//纹理坐标步进(向量)
   float desnityAcc=0.0;//累加的密度值
   for(int i=0;i<count;i++)//循环采样叠加
   {
		if(desnityAcc*myConstantValsFrag.LFactor>myConstantValsFrag.detailDepth)continue;//若累积密度超过指定阈值则不能继续透过
		vec3 currTexCoor=start+stepV*i;//当前步的纹理坐标
		if(currTexCoor.x<myConstantValsFrag.startX||currTexCoor.x>myConstantValsFrag.endX||
		   currTexCoor.y<myConstantValsFrag.startY||currTexCoor.y>myConstantValsFrag.endY||
		   currTexCoor.z<myConstantValsFrag.startZ||currTexCoor.z>myConstantValsFrag.endZ)//若纹理坐标不在扫描范围内则略过
		{
			continue;
		}
		float desnityCurr=texture(tex3DVolume,currTexCoor).r;//当前采样点采样值
		desnityCurr=desnityCurr/float(count);//当前点采样值按照总步数衰减
		desnityAcc=(1-desnityAcc)*desnityCurr+desnityAcc;//累积采样值
   }
   desnityAcc=desnityAcc*myConstantValsFrag.LFactor;//密度范围调整，便于观察效果
   float LD=1.0-min(1.0,desnityAcc);//将密度值换算为亮度值(密度越大亮度越小)
   outColor.rgb=texture(tex2DPalette,vec2(LD,0.5)).rgb;//通过亮度值从调色板取颜色得到最终颜色值
   outColor.a=1.0;//结果Alpha 通道的值
}