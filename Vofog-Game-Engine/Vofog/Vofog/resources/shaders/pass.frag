#version 430 core

#define VOFOG_TEXTURE_SLOTS 32

out vec4 FragColor;


in vec3 oNormal;

in VS_OUT_FG_IN
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

} fg_in;

uniform sampler2D u_textures[VOFOG_TEXTURE_SLOTS];

uniform vec3 lightPos; 
uniform vec3 lightColor;

uniform vec3 u_viewPos;

void main(){ 
    int diffuseIndex = int(fg_in.texIDs.x + 0.5);
    int specularIndex = int(fg_in.texIDs.y + 0.5);
    vec3 sampledImage = vec3(fg_in.color);
    float amp= 1;
    if(diffuseIndex != 0)
        sampledImage *= texture(u_textures[diffuseIndex], fg_in.texCoords).rgb;

         // ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(oNormal);
    vec3 lightDir = normalize(lightPos - fg_in.position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    float attDistance = 1;
    float attenuation = pow(max(1 - pow(attDistance/1.2f,4),0),2) 
						* (1 / (0.1f+(attDistance * attDistance)));

    // specular
    vec3 viewDir = normalize(u_viewPos - fg_in.position);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.9 * spec * lightColor;
    if(specularIndex != 0 )
        specular *= vec3(texture(u_textures[specularIndex], fg_in.texCoords));  


	//diffuse*= attenuation;
    vec3 result = (ambient + diffuse + specular) * sampledImage;
    FragColor = vec4(result, 1.0);
   // FragColor = vec4(sampledImage * fg_in.color.rgb * amp, 1);

}