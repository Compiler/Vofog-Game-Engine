#version 460 core

in vec4 geo_oColor;

out vec4 fragColor;

void main(){
	vec3 col = abs(geo_oColor.xyz);
	if(geo_oColor.x < 0)col.x = abs(geo_oColor.x) / 2.0;
	if(geo_oColor.y < 0)col.y = abs(geo_oColor.y) / 2.0;
	if(geo_oColor.z < 0) col.z = abs(geo_oColor.z) / 2.0;
	fragColor = vec4(col,1);
} 