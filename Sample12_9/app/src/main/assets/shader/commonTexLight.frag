#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec2 vTexPosition;
layout (location = 1) in vec4 inLightQD;
layout (location = 0) out vec4 outColor;
const float maxIterations = 99.0;//最大迭代次数
const float zoom = 0.02;//缩放系数
const float xCenter = -0.0002;//中心坐标位置
const float yCenter = 0.7383;
const vec3 innerColor = vec3(0.0, 0.0, 1.0);//内部颜色
const vec3 outerColor1 = vec3(1.0, 0.0, 0.0);//外部颜色1
const vec3 outerColor2 = vec3(0.0, 1.0, 0.0);//外部颜色2
void main() {
    float real = vTexPosition.x * zoom + xCenter;//变换当前位置
    float imag = vTexPosition.y * zoom + yCenter;
    float cReal = real;//c的实部
    float cImag = imag;//c的虚部
    float r2 = 0.0;//半径的平方
    float i;//迭代次数
    for(i=0.0; i<maxIterations && r2<4.0; i++){//循环迭代
    	float tmpReal = real;//保存当前实部值
    	real = (tmpReal * tmpReal) - (imag * imag) +cReal;//计算下一次迭代后实部的值
    	imag = 2.0 *tmpReal * imag +cImag;//计算下一次迭代后虚部的值
    	r2 = (real * real) + (imag * imag);//计算半径的平方
    }
    vec3 color;
    if(r2 < 4.0){//如果r2未达到4就退出了循环，表明迭代次数已达到最大值
        color = innerColor;//为内部颜色赋值
    }else{//如果因r2大于4.0而退出循环，表明此位置在外部
        color = mix(outerColor1, outerColor2, fract(i * 0.07));//按迭代次数为外部颜色赋不同的值
    }
   outColor=vec4(color, 1.0)*inLightQD;
}