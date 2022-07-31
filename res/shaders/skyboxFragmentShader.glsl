#version 430 core

const float LOWER_LIMIT = 0.0f;
const float UPPER_LIMIT = 25.0f;

in vec3 txCoords;

uniform samplerCube cubeMap;
uniform vec3 fogColor;

out vec4 outColor;

void main()
{
	vec4 finalColor = texture(cubeMap, txCoords);
	float factor    = (txCoords.y - LOWER_LIMIT) / (UPPER_LIMIT - LOWER_LIMIT);
	factor          = clamp(factor, 0.0f, 1.0f);
	outColor        = mix(vec4(fogColor, 1.0f), finalColor, factor);
}