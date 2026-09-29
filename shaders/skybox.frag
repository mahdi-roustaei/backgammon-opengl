#version 330 core

in vec3 fragDirection;
out vec4 color;

uniform samplerCube skyboxCube;

void main()
{
    color = texture(skyboxCube, fragDirection);
}