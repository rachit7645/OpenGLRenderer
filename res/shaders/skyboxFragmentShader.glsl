#version 330 core

in vec3 textureCoords;

uniform samplerCube cubeMap;
uniform vec3 fogColor;

out vec4 out_Color;

const float LOWER_LIMIT = 0.0f;
const float UPPER_LIMIT = 30.0f;

void main()
{
	vec4 finalColor = texture(cubeMap, textureCoords);
	float factor = (textureCoords.y - LOWER_LIMIT) / (UPPER_LIMIT - LOWER_LIMIT);
	factor = clamp(factor, 0.0f, 1.0f);
	out_Color = mix(vec4(fogColor, 1.0f), finalColor, factor);
}