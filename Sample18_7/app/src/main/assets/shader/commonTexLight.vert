#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTexCoor;
layout (location = 2) in vec3 inNormal;
layout (location = 0) out vec3 tcsNormal;
layout (location = 1) out vec2 tcsTexCoor;
out gl_PerVertex { 
	vec4 gl_Position;
};
void main()
{
   	gl_Position = vec4(pos,1);
   	tcsNormal=inNormal;
   	tcsTexCoor=inTexCoor;
}
