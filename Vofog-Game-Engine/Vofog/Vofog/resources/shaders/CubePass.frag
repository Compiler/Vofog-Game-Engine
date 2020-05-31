#version 460 core

#define VOFOG_TEXTURE_SLOTS /*this will be updated before compiled with your hardwares options*/

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

out vec4 fragColor;


struct Material {
    sampler2D diffuse;
    sampler2D specular;
	sampler2D normalMap;
	sampler2D parallaxMap;
    float shininess;
}; 

struct VFG_PointLight {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	vec4 color;

	float intensity;

	//Attenuation
	float reach;
};



#define VFG_POINT_LIGHT_COUNT 2
#define VFG_SPOT_LIGHT_COUNT 1

uniform Material u_material;
uniform VFG_PointLight u_VFG_PointLight[VFG_POINT_LIGHT_COUNT];
uniform float u_brightness;
uniform bool u_normalMapping, u_visualizeTangentSpace, u_useParallax, u_useShadowVar, u_useLighting, u_useGammaModifier;

uniform float u_parallaxMinLayers, u_parallaxMaxLayers;

uniform sampler2D u_shadowMap;


float shadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	//float bias = 0.005;
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
	if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}  

vec3 pointLightCalculation(VFG_PointLight light, vec4 diffuseMap, vec4 specularMap, vec3 normal, vec3 viewDir){
	
	vec3 myMappedPosition = /*fg_in.position;*/fg_in.tangentFragPos;
	vec3 myLightPos = /*light.position;*/fg_in.tangentLightPos;
    vec3 ambient = diffuseMap.rgb * (light.ambient * light.color.rgb);
	
	//vec3 lightToFragDirection = normalize(light.position - myMappedPosition);  
	vec3 lightToFragDirection = normalize(myLightPos - myMappedPosition);
	vec3 halfwayDir = normalize(lightToFragDirection + viewDir);

	float diff = max(dot(normal, lightToFragDirection), 0.0);
	vec3 diffuse = (diff * diffuseMap.rgb) * (light.color.rgb * light.diffuse);

    vec3 reflectDir = reflect(-lightToFragDirection, normal);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), u_material.shininess);
    vec3 specular = (specularMap.rgb * spec) *  (light.color.rgb * light.specular);  

    float attDistance = length(myLightPos - myMappedPosition);
	
	//vec3 l = (light.position - fg_in.position) / length(light.position - fg_in.position);
	
	float attenuation = pow(max(1 - pow(attDistance/light.reach,4),0),2) 
						* (1 / (0.1f+(attDistance * attDistance)));

	diffuse*= attenuation; specular*= attenuation;ambient *= attenuation*attenuation;
	float shadow = 0;
	if(u_useShadowVar)
		shadow = shadowCalculation(fg_in.fragPosLightSpace, normal, lightToFragDirection); 
	return (ambient + (1.0f-shadow) * (diffuse + specular)) * fg_in.color.rgb * light.intensity;
}



vec2 computeParallaxCoords(vec3 viewDir, float scale){

// number of depth layers
	const float magg = 16/16;
    const float minLayers = u_parallaxMinLayers;
    const float maxLayers = u_parallaxMaxLayers; 
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * scale; 
    vec2 deltaTexCoords = P / numLayers;
	// get initial values
	vec2 currentTexCoords = fg_in.texCoords;
	float currentDepthMapValue = texture(u_material.parallaxMap, currentTexCoords).r;
  
	while(currentLayerDepth < currentDepthMapValue)
	{
		// shift texture coordinates along direction of P
		currentTexCoords -= deltaTexCoords;
		// get depthmap value at current texture coordinates
		currentDepthMapValue = texture(u_material.parallaxMap, currentTexCoords).r;  
		// get depth of next layer
		currentLayerDepth += layerDepth;  
	}

		// get texture coordinates before collision (reverse operations)
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(u_material.parallaxMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	return finalTexCoords;  

}



uniform sampler2D u_textures[VOFOG_TEXTURE_SLOTS];

void main(){
    vec3 viewDir = normalize(fg_in.tangentViewPos - fg_in.tangentFragPos);
	vec2 parallaxMap;
	parallaxMap = fg_in.texCoords;
	parallaxMap = computeParallaxCoords(viewDir, 0.075f);

	int diffuseIndex = int(fg_in.texIDs.x + 0.5);
	int specIndex = int(fg_in.texIDs.y + 0.5);
	vec4 diffuseMap = texture(u_textures[diffuseIndex], parallaxMap);
	vec4 specularMap = texture(u_textures[specIndex], parallaxMap); 


	
	vec3 norm = fg_in.matTBN[2];
	norm = normalize(texture(u_material.normalMap, parallaxMap).rgb * 2.0 - 1.0); 


    vec3 result = vec3(0);
	for(int i =0; i < VFG_POINT_LIGHT_COUNT; i++)
		result += pointLightCalculation(u_VFG_PointLight[i], diffuseMap, specularMap, norm, viewDir); 

	vec3 gamma = vec3(1.0);
	gamma = vec3(1.0f/2.2f);

	vec3 mapped = result / (result + vec3(1.0));

	fragColor.rgb = pow(mapped.rgb, gamma);

	


	
}	
