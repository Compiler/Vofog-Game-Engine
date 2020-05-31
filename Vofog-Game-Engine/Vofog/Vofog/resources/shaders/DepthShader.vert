#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_lightSpaceMatrix;

void main()
{
    gl_Position = u_lightSpaceMatrix * vec4(aPos, 1.0);
}  