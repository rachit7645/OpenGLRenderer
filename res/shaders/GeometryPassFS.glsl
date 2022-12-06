#version 430 core

const float GAMMA_FACTOR = 2.2f;

layout (location = 0) out vec2 gNormal;
layout (location = 1) out vec3 gAlbedo;
layout (location = 2) out vec3 gMaterial;

in vec2 txCoords;
in vec3 worldPos;
in vec3 unitNormal;
in mat3 TBNMatrix;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D aoRghMtl;

vec3 GetNormalFromMap(vec3 normal);
vec2 PackNormal(vec3 normal);

void main()
{
	// Retrieve data
	vec4 gAlb      = texture(albedoMap, txCoords);
	vec4 gNorm     = texture(normalMap, txCoords);
	vec4 gAoRghMtl = texture(aoRghMtl,  txCoords);
	// Normal
	gNormal.rg = PackNormal(GetNormalFromMap(gNorm.rgb));
	// Albedo
	gAlbedo = pow(gAlb.rgb, vec3(GAMMA_FACTOR));
	// AO + Roughness + Metallic
	gMaterial = gAoRghMtl.rgb;
}

vec3 GetNormalFromMap(vec3 normal)
{
	// Tangent normal from normal map
	vec3 tangentNormal = normal * 2.0f - 1.0f;
	// Return world space normal
	return normalize(TBNMatrix * tangentNormal);
}

vec2 PackNormal(vec3 normal)
{
	// Pack normal
	float p      = sqrt(normal.z * 8.0f + 8.0f);
	vec2  result = normal.xy / p + 0.5f;
	// Return
	return result;
}