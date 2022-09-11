#version 430 core

in vec2 txCoords;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

out vec4 outColor;

void main()
{
	outColor = texture(diffuseTexture, txCoords) + texture(specularTexture, txCoords);
}