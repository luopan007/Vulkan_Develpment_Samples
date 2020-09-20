#version 400
#extension GL_ARB_separate_shader_objects : enable//启动GL_ARB_separate_shader_objects
#extension GL_ARB_shading_language_420pack : enable//启动GL_ARB_shading_language_420pack
layout (std140,set = 0, binding = 0) uniform bufferVals { //一致变量块
    float brightFactor; //亮度调节系数
} myBufferVals;
layout (location = 0) in vec3 vPosition; //顶点着色器传入的位置坐标数据
layout (location = 0) out vec4 outColor; //输出到渲染管线的片元颜色值
void main() { //主函数
    vec4 bColor=vec4(0.678,0.231,0.129,0); //深红色
    vec4 mColor=vec4(0.763,0.657,0.614,0); //淡红色
    float y=vPosition.y; //提取当前片元的y 坐标值
    y=mod((y+100.0)*4.0,4.0); //折算出区间值
    if(y>1.8) { //判断区间值是否大于指定值
        outColor = bColor; //设置片元颜色为深红色
    }else{ //若区间值不大于指定值
        outColor = mColor; //设置片元颜色为淡红色
    }
    outColor=myBufferVals.brightFactor*outColor; //计算片元最终颜色值
}
