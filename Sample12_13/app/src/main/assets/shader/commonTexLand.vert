#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (push_constant) uniform constantVals {
	mat4 mvp;
	mat4 mm;
} myConstantVals;
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTexCoor;
layout (location = 0) out vec2 outTexCoor;
layout (location = 1) out float landHeight;
layout (location = 2) out vec4 pLocation;
out gl_PerVertex {
	vec4 gl_Position;
};
void main() {
    outTexCoor = inTexCoor;//传入纹理坐标
    landHeight=pos.y;//传入当前的高度（计算多重纹理时使用）
    gl_Position = myConstantVals.mvp * vec4(pos,1.0);//计算最终顶点位置
    pLocation=myConstantVals.mm * vec4(pos,1.0);//计算世界坐标系下顶点位置
}
