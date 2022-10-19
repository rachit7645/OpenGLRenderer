#version 430 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 txCoords;
in vec3 unitNormal;
in vec3 worldPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

void main()
{
	gPosition       = worldPosition;
	gNormal         = unitNormal;
	gAlbedoSpec.rgb = texture(diffuseTexture, txCoords).rgb;
	gAlbedoSpec.a   = texture(specularTexture, txCoords).r;
}

