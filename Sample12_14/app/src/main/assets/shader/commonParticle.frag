#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex; //传入的纹理
layout (push_constant) uniform constantValsFrag{ //推送常量块
    layout(offset=64)vec4 startColor; //粒子系统起始颜色
    layout(offset=80)vec4 endColor; //粒子系统终止颜色
    layout(offset=96) float bj; //粒子纹理矩形半径
} myConstantValsFrag;
layout (location = 0) in vec2 inTexCoor; //输入的纹理坐标
layout (location = 1) in float sjFactor; //输入的总衰减因子
layout (location = 2) in vec4 vPosition; //输入的顶点各项参数
layout (location = 0) out vec4 outColor; //输出的最终片元颜色
void main() {
    if(vPosition.w==10.0){ //该片元的生命期为10.0 时，处于未激活状态，不绘制
    outColor=vec4(0.0,0.0,0.0,0.0); //舍弃此片元
    }else{ //该片元的生命期不为10.0 时，处于活跃状态，绘制
        const float newBJ=myConstantValsFrag.bj*60.0f; //计算新的半径
        vec4 colorTL = textureLod(tex, inTexCoor, 0.0); //采样纹理
        vec4 colorT; //颜色变量
        float disT=distance(vPosition.xyz,vec3(0.0,0.0,0.0)); //计算当前片元与中心点的距离
        float tampFactor=(1.0-disT/newBJ)*sjFactor; //计算片元颜色插值因子
        vec4 factor4=vec4(tampFactor,tampFactor,tampFactor,tampFactor);
        colorT=clamp(factor4,myConstantValsFrag.endColor,myConstantValsFrag.startColor);//进行颜色插值
        colorT=colorT*colorTL.a; //结合采样出的透明度计算出最终颜色
        outColor=colorT; //将计算出来的最终片元颜色传给渲染管线
    }
}
