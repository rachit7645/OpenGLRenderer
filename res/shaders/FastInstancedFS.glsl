#version 430 core

in vec2 txCoords;
in vec4 vertexColor;

uniform sampler2D diffuseTexture;

out vec4 outColor;

void main()
{
	outColor = vertexColor * texture(diffuseTexture, txCoords);
}