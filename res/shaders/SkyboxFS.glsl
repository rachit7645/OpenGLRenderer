#version 430 core

in vec3 txCoords;

uniform samplerCube cubeMap;

out vec3 outColor;

void main()
{
	// Sample cube map
	outColor = texture(cubeMap, txCoords).rgb;
}