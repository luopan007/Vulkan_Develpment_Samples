#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 0) uniform sampler2D tex;
layout (std140,set = 0, binding = 1) uniform bufferVals { //一致块
vec4 uCamera; //摄像机位置
} myBufferVals;
layout (location = 0) in vec2 inTexCoor;
layout (location = 1) in vec3 inPos;
layout (location = 2) in vec3 inNormal;
layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outPos;
layout (location = 2) out vec4 outNormal;
layout (location = 3) out vec4 outFragDis; //该片元的世界空间坐标位置到摄像机的距离
void main() { //主函数
   outColor=texture(tex, inTexCoor);
   outPos=vec4(inPos.xyz,1.0);
   outNormal=vec4(inNormal.xyz,1.0);
   outFragDis= //计算该片元的世界空间坐标位置到离摄像机的距离并输出
                                    vec4(distance(myBufferVals.uCamera.xyz,inPos),0.0,0.0,0.0);
}
