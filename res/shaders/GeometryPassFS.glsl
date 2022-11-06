#version 430 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gNormalMap;

in vec2 txCoords;
in vec3 unitNormal;
in vec3 worldPosition;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D mtlRgh;
uniform sampler2D aoMap;

void main()
{
	// Position + Metallic
	gPosition.rgb = worldPosition;
	gPosition.a   = texture(mtlRgh, txCoords).b;
	// Normal + Roughness
	gNormal.rgb = unitNormal;
	gNormal.a   = texture(mtlRgh, txCoords).g;
	// Albedo
	gAlbedo = pow(texture(albedoMap, txCoords), vec4(vec3(2.2f), 1.0f));
	// Normal Map + Ambient Occlusion
	gNormalMap.rgb = texture(normalMap, txCoords).rgb;
	gNormalMap.a   = texture(aoMap,     txCoords).r;
}