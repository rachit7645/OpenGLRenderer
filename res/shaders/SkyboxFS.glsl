#version 430 core

// Vertex inputs
in vec3 txCoords;

// Uniforms
uniform samplerCube cubeMap;

// Fragment outputs
layout (location = 0) out vec3 outColor;

// Entry point
void main()
{
	// Sample cube map
	outColor = texture(cubeMap, txCoords).rgb;
}