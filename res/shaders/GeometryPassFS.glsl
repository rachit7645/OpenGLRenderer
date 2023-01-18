#version 450 core

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
vec2 OctWrap(vec2 vector);
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

// FIXME: Remove conditionals
vec2 OctWrap(vec2 vector)
{
	// Calculate wrapped vector
	vec2 wrapped = 1.0f - abs(vector.yx);

	// X check
	if (vector.x < 0.0f)
	{
		wrapped.x = -wrapped.x;
	}

	// Y check
	if (vector.y < 0.0f)
	{
		wrapped.y = -wrapped.y;
	}

	// Return wrapped vector
	return wrapped;
}

// FIXME: Remove conditionals
vec2 PackNormal(vec3 normal)
{
	// Division
	normal /= abs(normal.x) + abs(normal.y) + abs(normal.z);
	// Wrap check
	normal.xy = normal.z > 0.0f ? normal.xy : OctWrap(normal.xy);
	// Convert range
	normal.xy = normal.xy * 0.5f + 0.5f;
	// Return packed normal
	return normal.xy;
}