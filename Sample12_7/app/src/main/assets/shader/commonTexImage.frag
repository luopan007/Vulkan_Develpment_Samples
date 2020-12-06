#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D uImageUnit;
layout (location = 0) in vec2 vST;
layout (location = 0) out vec4 outColor;
void main() {
        float uMagTol=0.2f;//设定的阈值，判定当前点是否为边缘点
        float uQuantize=18.0f;//阈值量化因子
        ivec2 ires=textureSize(uImageUnit,0);		//获得纹理图的宽高度
    	float uResS=float(ires.s);					//获得纹理图的S值
    	float uResT=float(ires.t);					//获得纹理图的T值
    	vec3 rgb=texture(uImageUnit,vST).rgb;		//获得纹理采样的rgb值
    	vec2 stp0=vec2(1.0/uResS,0.0);			//与左右相邻像素间的距离向量
    	vec2 st0p=vec2(0.0,1.0/uResT);			//与上下相邻像素间的距离向量
    	vec2 stpp=vec2(1.0/uResS,1.0/uResT);		//与左下、右上相邻像素间的距离向量
    	vec2 stpm=vec2(1.0/uResS,-1.0/uResT);		//与左上、右下相邻像素间的距离向量
    	const vec3 W=vec3(0.2125,0.7154,0.0721);	//绿色
    	//当前像素右下、左下、右上、左下相邻点的灰度值
    	float im1m1=dot( texture( uImageUnit,vST-stpp).rgb,W );
    	float ip1p1=dot( texture( uImageUnit,vST+stpp).rgb,W );
    	float im1p1=dot( texture( uImageUnit,vST-stpm).rgb,W );
    	float ip1m1=dot( texture( uImageUnit,vST+stpm).rgb,W );
    	float im10=dot( texture( uImageUnit,vST-stp0).rgb,W );//左边相邻点的灰度值
    	float ip10=dot( texture( uImageUnit,vST+stp0).rgb,W );//右边相邻点的灰度值
    	float i0m1=dot( texture( uImageUnit,vST-st0p).rgb,W );//上边相邻点的灰度值
    	float i0p1=dot( texture( uImageUnit,vST+st0p).rgb,W );//下边相邻的点的灰度值
    	//与图像作平面卷积计算，分别得出横向及纵向的亮度差分近似值，即sobel算子的横纵灰度值
    	float h = -1.0 * im1p1 - 2.0 * i0p1 - 1.0 * ip1p1 + 1.0 * im1m1 + 2.0 * i0m1 + 1.0 * ip1m1;
    	float v = -1.0 * im1m1 - 2.0 * im10 - 1.0 * im1p1 + 1.0 * ip1m1 + 2.0 * ip10 + 1.0 * ip1p1;
    	float mag=length(vec2(h,v));//当前像素点的梯度值
    	if(mag>uMagTol){//如果梯度mag大于阈值，则认为该点为边缘点，最终颜色为黑色
    		outColor=vec4(0.0,0.0,0.0,1.0);
    	}else{//不在物体边缘,量化物体的颜色值
    		rgb.rgb *= uQuantize; //将当前片元的颜色值乘以量化值
    		rgb.rgb += vec3(0.5,0.5,0.5);					//卡通化程度
    		ivec3 intrgb = ivec3(rgb.rgb);					//转换成整数类型的向量
    		rgb.rgb = vec3(intrgb) / uQuantize; 			//将整数类型的片元颜色值除以量化值
    		outColor=vec4(rgb,1.0);					//获得重新计算的最终颜色值
    	}
}