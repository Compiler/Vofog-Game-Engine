#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBiTangent;
layout (location = 6) in vec4 aTexIDs;//diffuse, specular, normal, height


out VS_OUT_FG_IN
{
    vec2 texCoords;
	vec3 position;
	//vec3 normals;
	vec4 color;
	mat3 matTBN;

	vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;

	vec4 fragPosLightSpace;


	vec4 texIDs;
} vs_out;


uniform mat4 u_vertexMVP;
uniform vec3 u_lightPosition, u_viewPos;
uniform mat4 u_lightSpaceMatrix;


void main(){
	vs_out.texIDs = aTexIDs;
	vs_out.texCoords = aTexCoord;
	vs_out.position = aPos;
	//vs_out.normals = normalize(aNormal);
	vs_out.color = aColor;

	vec3 tangent = normalize(aTangent - dot(aTangent, aNormal) * aNormal);
	vec3 biTangent = normalize(cross(aNormal, tangent));

	vs_out.matTBN = mat3(aTangent, biTangent, aNormal);
	mat3 TBN = transpose(mat3(tangent, biTangent, aNormal));    
    vs_out.tangentLightPos = TBN * u_lightPosition;
    vs_out.tangentViewPos  = TBN * u_viewPos;
    vs_out.tangentFragPos  = TBN * aPos;

	vs_out.fragPosLightSpace = u_lightSpaceMatrix * vec4(vs_out.position, 1.0);




	gl_Position = u_vertexMVP * vec4(aPos, 1.0);
}