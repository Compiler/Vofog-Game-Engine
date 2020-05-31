#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D u_depthMap;
void main()
{             
    float depthValue = texture(u_depthMap, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}  