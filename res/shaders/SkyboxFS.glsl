#version 430 core

in vec3 txCoords;

uniform samplerCube cubeMap;

out vec4 outColor;

void main()
{
	// Sample cube map
	outColor = texture(cubeMap, txCoords);
	// HDR tonemapping
	outColor = outColor / (outColor + vec4(vec3(1.0f), 0.0f));
	// Gamma correction
	outColor = pow(outColor, vec4(vec3(1.0f / 2.2f), 1.0f));
}