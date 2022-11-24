#version 430 core

const float GAMMA_FACTOR = 2.2f;

layout (location = 0) out vec2 gNormal;
layout (location = 1) out vec3 gAlbedo;
layout (location = 2) out vec3 gMaterial;

in vec2 txCoords;
in vec3 worldPos;
in vec3 unitNormal;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D aoMtlRgh;

vec3 GetNormalFromMap(vec3 normal);
vec2 PackNormal(vec3 normal);

void main()
{
	// Retrieve data
	vec4 gAlb      = texture(albedoMap, txCoords);
	vec4 gNorm     = texture(normalMap, txCoords);
	vec4 gAoMtlRgh = texture(aoMtlRgh,  txCoords);
	// Normal
	gNormal.rg = PackNormal(GetNormalFromMap(gNorm.rgb));
	// Albedo
	gAlbedo = pow(gAlb.rgb, vec3(GAMMA_FACTOR));
	// AO + Roughness + Metallic
	gMaterial = gAoMtlRgh.rgb;
}

vec3 GetNormalFromMap(vec3 normal)
{
	// Tangent normal from normal map
	vec3 tangentNormal = normal * 2.0f - 1.0f;

	// Take the deriviatives
	vec3 Q1  = dFdx(worldPos);
	vec3 Q2  = dFdy(worldPos);
	vec2 st1 = dFdx(txCoords);
	vec2 st2 = dFdy(txCoords);

	// Calculate TBN matrix
	vec3 N   = unitNormal;
	vec3 T   = normalize(Q1 * st2.t - Q2 * st1.t);
	vec3 B   = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	// Return world space normal
	return normalize(TBN * tangentNormal);
}

vec2 PackNormal(vec3 normal)
{
	// Pack normal
	float p      = sqrt(normal.z * 8.0f + 8.0f);
	vec2  result = normal.xy / p + 0.5f;
	// Return
	return result;
}