#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 oColor;

uniform mat4 u_vertexMVP;

void main(){
	oColor = aColor;
	gl_Position = u_vertexMVP * vec4(aPos.xyz, 1.0);
}