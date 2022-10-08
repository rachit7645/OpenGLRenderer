#version 430 core

in vec3 txCoords;

uniform samplerCube cubeMap;

out vec4 outColor;

void main()
{
	outColor = texture(cubeMap, txCoords);
}