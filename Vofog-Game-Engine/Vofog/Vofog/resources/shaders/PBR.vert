#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 6) in float aTexID;


out VS_OUT_FG_IN
{
    vec2 texCoords;
	vec3 position;
	vec4 color;
	float texID;
} vs_out;


uniform mat4 u_vertexMVP;

void main(){
	vs_out.texID = aTexID;
	vs_out.texCoords = aTexCoord;
	vs_out.position = aPos;
	vs_out.color = aColor;
	gl_Position = u_vertexMVP * vec4(aPos, 1.0);
}