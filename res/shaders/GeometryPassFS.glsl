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
layout (location = 0) out vec4 gNormal;
layout (location = 1) out vec4 gAlbedo;
layout (location = 2) out vec3 gEmmisive;

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
	// Normal + AO + Roughness
	gNormal.rg = PackNormal(GetNormalFromMap(gNrm.rgb));
	gNormal.b  = gMat.r;
	gNormal.a  = gMat.g;
	// Albedo + Metallic
	gAlbedo.rgb = pow(gAlb.rgb, vec3(GAMMA_FACTOR));
	gAlbedo.a   = gMat.b;
	// Emmisive color
	gEmmisive = pow(gEmm.rgb, vec3(GAMMA_FACTOR));
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