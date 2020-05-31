#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBiTangent;

//out vec3 oPos;
out vec4 oNormal;
out mat3 oTBN;

uniform mat4 u_vertexMVP;



void main(){
	
	oNormal = vec4(aNormal,0);

	oNormal = normalize(u_vertexMVP * vec4(aNormal,0));
	oTBN =  mat3(aTangent, aBiTangent, oNormal);
	//oPos = vec3((u_vertexMVP * vec4(aPos, 1.0)).xyz);
	gl_Position = u_vertexMVP * vec4(aPos, 1.0);
}