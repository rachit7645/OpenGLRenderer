#version 430 core

// GBuffer Layout
// Buffer     | Type      | R           | G           | B           | A
// Normal     | RGBA_16F  | normal.x    | normal.y    | normal.z    | roughness
// Albedo     | RGBA_16F  | albedo.r    | albedo.g    | albedo.b    | metallic
// Normal Map | RGBA_16F  | normalMap.r | normalMap.g | normalMap.b | ambient occlusion
// Depth      | DEPTH_24F | depth       | NONE        | NONE        | NONE

const float GAMMA_FACTOR = 2.2f;

layout (location = 0) out vec4 gNormal;
layout (location = 1) out vec4 gAlbedo;
layout (location = 2) out vec4 gNormalMap;

in vec2 txCoords;
in vec3 unitNormal;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D aoMtlRgh;

void main()
{
	// Retrieve data
	vec4 gAlb      = texture(albedoMap, txCoords);
	vec4 gNorm     = texture(normalMap, txCoords);
	vec4 gAoMtlRgh = texture(aoMtlRgh,  txCoords);
	// Normal + Roughness
	gNormal.rgb = unitNormal;
	gNormal.a   = gAoMtlRgh.g;
	// Albedo + Metallic
	gAlbedo.rgb = pow(gAlb.xyz, vec3(GAMMA_FACTOR));
	gAlbedo.a   = gAoMtlRgh.b;
	// Normal Map + Ambient Occlusion
	gNormalMap.rgb = gNorm.rgb;
	gNormalMap.a   = gAoMtlRgh.r;
}