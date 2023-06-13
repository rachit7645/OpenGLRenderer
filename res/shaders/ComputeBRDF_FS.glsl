#version 430 core

// Constants
const vec3  N            = vec3(0.0f, 0.0f, 1.0f);
const float PI           = 3.14159265359;
const uint  SAMPLE_COUNT = 2048u;

// Vertex inputs
in vec2 txCoords;

// Fragment outputs
layout (location = 0) out vec2 outColor;

// Integration function
vec2 IntegrateBRDF(float NdotV, float roughness);

// Sampling functions
float RadicalInverse_VdC(uint bits);
vec2  Hammersley(uint i, uint N);

// PBR functions
vec3  ImportanceSampleGGX(vec2 Xi, vec3 N, vec3 tangent, vec3 bitangent, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

// Entry point
void main()
{
	// Output integrant
	outColor = IntegrateBRDF(txCoords.x, txCoords.y);
}

vec2 IntegrateBRDF(float NdotV, float roughness)
{
	// Convert from tangent to world space
	vec3 up        = vec3(1.0f, 0.0f, 0.0f);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

	// Get view direction
	vec3 V = vec3
	(
		sqrt(1.0f - NdotV * NdotV),
		0.0f,
		NdotV
	);

	// Store A & B
	float A = 0.0f;
	float B = 0.0f;

	// For sample count
	for(uint i = 0u; i < SAMPLE_COUNT; ++i)
	{
		// Importance Sampling
		vec2 Xi = Hammersley(i, SAMPLE_COUNT);
		vec3 H  = ImportanceSampleGGX(Xi, N, tangent, bitangent, roughness);
		vec3 L  = normalize(2.0f * dot(V, H) * H - V);

		// Dot products
		float NdotL = max(L.z, 0.0f);
		float NdotH = max(H.z, 0.0f);
		float VdotH = max(dot(V, H), 0.0f);

		// To prevent NAN operations
		if(NdotL > 0.0f)
		{
			// PBR calculation
			float G     = GeometrySmith(N, V, L, roughness);
			float G_Vis = (G * VdotH) / (NdotH * NdotV);
			float Fc    = pow(1.0f - VdotH, 5.0f);

			// Add
			A += (1.0f - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}

	// Divide by sample count
	A /= float(SAMPLE_COUNT);
	B /= float(SAMPLE_COUNT);

	// Return
	return vec2(A, B);
}

float RadicalInverse_VdC(uint bits)
{
	// I have no idea how this works
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	// Return
	return float(bits) * 2.3283064365386963e-10;
}

vec2 Hammersley(uint i, uint N)
{
	// Do sampling and return
	return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, vec3 tangent, vec3 bitangent, float roughness)
{
	// Using Disney squared roughness
	float a = roughness * roughness;

	// Calculate trigonometric ratios
	float phi      = 2.0f * PI * Xi.x;
	float cosTheta = sqrt((1.0f - Xi.y) / (1.0f + (a * a - 1.0f) * Xi.y));
	float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

	// Calculate spherical to cartesian halfway vector
	vec3 H = vec3
	(
		cos(phi) * sinTheta,
		sin(phi) * sinTheta,
		cosTheta
	);

	// Create sample vector
	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	// Return normalised
	return normalize(sampleVec);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	// Not using Pixar roughness
	float a = roughness;
	// Different k for IBL
	float k = (a * a) / 2.0f;

	// Calculate fraction terms
	float nom   = NdotV;
	float denom = NdotV * (1.0f - k) + k;

	// Return division
	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	// Dot products
	float NdotV = max(dot(N, V), 0.0f);
	float NdotL = max(dot(N, L), 0.0f);
	// Different geometry calculations
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	// Return multiplication
	return ggx1 * ggx2;
}