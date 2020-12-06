#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 1) uniform sampler2D tex;
layout (push_constant) uniform constantValsFrag{
    layout(offset=128)vec4 startColor;
    layout(offset=144)vec4 endColor;
    layout(offset=160) float bj;
} myConstantValsFrag;
layout (location = 0) in float sjFactor;
layout (location = 1) in vec4 vPosition;
layout (location = 0) out vec4 outColor;
void main() {
    if(vPosition.w==10.0){
		outColor=vec4(0.0,0.0,0.0,0.0);
	}else{
      vec4 colorTL =textureLod(tex,gl_PointCoord,0.0); //进行纹理采样
      vec4 colorT;
      float disT=distance(vPosition.xyz,vec3(0.0,0.0,0.0));
      float tampFactor=(1.0-disT/myConstantValsFrag.bj)*sjFactor;
      vec4 factor4=vec4(tampFactor,tampFactor,tampFactor,tampFactor);
      colorT=clamp(factor4,myConstantValsFrag.endColor,myConstantValsFrag.startColor);
      colorT=colorT*colorTL.a;
      outColor=colorT ;
	}
}