#version 430 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec4 gNormalMap;

in vec2 txCoords;
in vec3 unitNormal;
in vec3 worldPosition;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D aoMtlRgh;

void main()
{
	// Retrieve data
	vec4 gAlb      = texture(albedoMap, txCoords);
	vec4 gNorm     = texture(normalMap, txCoords);
	vec4 gAoMtlRgh = texture(aoMtlRgh,  txCoords);
	// Position + Metallic
	gPosition.rgb = worldPosition;
	gPosition.a   = gAoMtlRgh.b;
	// Normal + Roughness
	gNormal.rgb = unitNormal;
	gNormal.a   = gAoMtlRgh.g;
	// Albedo
	gAlbedo = pow(gAlb.xyz, vec3(2.2f));
	// Normal Map + Ambient Occlusion
	gNormalMap.rgb = gNorm.rgb;
	gNormalMap.a   = gAoMtlRgh.r;
}