#version 430 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;

in vec2 txCoords;
in vec3 unitNormal;
in vec3 worldPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform float shineDamper;
uniform float reflectivity;

void main()
{
	// Position + Shine Damper
	gPosition.rgb = worldPosition;
	gPosition.a   = shineDamper;
	// Normal + Reflectivity
	gNormal.rgb   = unitNormal;
	gNormal.a     = reflectivity;
	// Albedo + Specular
	gAlbedo.rgb   = texture(diffuseTexture,  txCoords).rgb;
	gAlbedo.a     = texture(specularTexture, txCoords).r;
}