#version 430 core

const float PI           = 3.14159265359;
const uint  SAMPLE_COUNT = 1024u;

in vec3 worldPos;

uniform samplerCube envMap;
uniform float       roughness;

out vec4 outColor;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float RadicalInverse_VdC(uint bits);
vec2  Hammersley(uint i, uint N);
vec3  ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);

void main()
{
	vec3 N = normalize(worldPos);

	// Assume that V equals R equals the normal
	vec3 R = N;
	vec3 V = R;

	vec3  prefilteredColor = vec3(0.0f);
	float totalWeight      = 0.0f;

	for(uint i = 0u; i < SAMPLE_COUNT; ++i)
	{
		// Importance Sampling
		vec2 Xi = Hammersley(i, SAMPLE_COUNT);
		vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
		vec3 L  = normalize(2.0f * dot(V, H) * H - V);

		float NdotL = max(dot(N, L), 0.0f);

		if(NdotL > 0.0f)
		{
			// Sample based on roughness/pdf
			float D     = DistributionGGX(N, H, roughness);
			float NdotH = max(dot(N, H), 0.0f);
			float HdotV = max(dot(H, V), 0.0f);
			float pdf   = D * NdotH / (4.0f * HdotV) + 0.0001f;

			float resolution = 512.0f;
			float saTexel    = 4.0f * PI / (6.0f * resolution * resolution);
			float saSample   = 1.0f / (float(SAMPLE_COUNT) * pdf + 0.0001f);

			float mipLevel = roughness == 0.0f ? 0.0f : 0.5f * log2(saSample / saTexel);

			prefilteredColor += textureLod(envMap, L, mipLevel).rgb * NdotL;
			totalWeight      += NdotL;
		}
	}

	prefilteredColor = prefilteredColor / totalWeight;
	outColor         = vec4(prefilteredColor, 1.0f);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a      = roughness * roughness;
	float a2     = a * a;
	float NdotH  = max(dot(N, H), 0.0f);
	float NdotH2 = NdotH * NdotH;

	float nom   = a2;
	float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
	denom       = PI * denom * denom;

	return nom / denom;
}

float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10;
}

vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness * roughness;

	float phi      = 2.0f * PI * Xi.x;
	float cosTheta = sqrt((1.0f - Xi.y) / (1.0f + (a * a - 1.0f) * Xi.y));
	float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

	// Spherical to Cartesian Halfway Vector
	vec3 H = vec3
	(
		cos(phi) * sinTheta,
		sin(phi) * sinTheta,
		cosTheta
	);

	// Tangent to World Space Sample Vector
	vec3 up        = abs(N.z) < 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}