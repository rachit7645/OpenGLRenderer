#version 430 core

// Constants
const float GAMMA_FACTOR = 2.2f;

// Vertex inputs
in vec2 txCoords;
in vec3 worldPos;
in mat3 TBNMatrix;

// Uniforms
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D aoRghMtlMap;
uniform sampler2D emmisiveMap;

// Fragment outputs
layout (location = 0) out vec2 gNormal;
layout (location = 1) out vec3 gAlbedo;
layout (location = 2) out vec3 gEmmisive;
layout (location = 3) out vec3 gMaterial;

// Normal functions
vec3 GetNormalFromMap(vec3 normal);
vec2 PackNormal(vec3 normal);

void main()
{
	// Fetch data from textures
	vec4 gAlb = texture(albedoMap,   txCoords);
	vec4 gNrm = texture(normalMap,   txCoords);
	vec4 gEmm = texture(emmisiveMap, txCoords);
	vec4 gMat = texture(aoRghMtlMap, txCoords);
	// Normal
	gNormal.rg = PackNormal(GetNormalFromMap(gNrm.rgb));
	// Albedo
	gAlbedo = pow(gAlb.rgb, vec3(GAMMA_FACTOR));
	// Emmisive color
	gEmmisive = pow(gEmm.rgb, vec3(GAMMA_FACTOR));
	// AO + Roughness + Metallic
	gMaterial = gMat.rgb;
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