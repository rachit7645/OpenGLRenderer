#version 430 core

// Constants
const int   MAX_KERNEL_SIZE = 64;
const float RADIUS          = 0.5f;
const float BIAS            = 0.025f;

// Matrix buffer
layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

// Shared buffer
layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
	vec4 cameraPos;
	vec4 resolution;
};

// SSAO buffer
layout(std140, binding = 5) uniform SSAOBuffer
{
	int  kernelSize;
	vec4 samples[MAX_KERNEL_SIZE];
};

// Vertex inputs
in      vec2 txCoords;
in flat mat4 invProj;
in flat mat4 invView;

// Uniforms
uniform sampler2D gNormal;
uniform sampler2D gDepth;
uniform sampler2D texNoise;

// Fragment outputs
layout(location = 0) out float outColor;

// Memory compression functions
vec3 ReconstructPosition(vec2 coords);
vec3 UnpackNormal(vec2 pNormal);

// Entry point
void main()
{
	// Get noise scale
	vec2 noiseScale = vec2(resolution.x / 4.0f, resolution.y / 4.0f);

	// Reconstruct position
	vec3 fragPos = ReconstructPosition(txCoords);
	// Unpack normal
	vec3 normal = UnpackNormal(texture(gNormal, txCoords).rg);
	// Get noise vector
	vec3 randomVec = normalize(texture(texNoise, txCoords * noiseScale).xyz);

	// Create TBN matrix
	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normal);

	// Store occlusion
	float occlusion = 0.0f;

	// Loop over kernels
	for(int i = 0; i < kernelSize; ++i)
	{
		// Get sample position
		vec3 samplePos = TBN * samples[i].xyz;
		samplePos      = fragPos + samplePos * RADIUS;

		// Project sample position
		vec4 offset = vec4(samplePos, 1.0f);
		offset      = projectionMatrix * offset;
		offset.xyz /= offset.w;
		offset.xyz  = offset.xyz * 0.5f + 0.5f;

		// Get sample depth
		float sampleDepth = ReconstructPosition(offset.xy).z;

		// Range check and accumulate
		float rangeCheck = smoothstep(0.0f, 1.0f, RADIUS / abs(fragPos.z - sampleDepth));
		occlusion       += (sampleDepth >= samplePos.z + BIAS ? 1.0f : 0.0f) * rangeCheck;
	}

	// Finalise occlusion
	occlusion = 1.0f - (occlusion / kernelSize);
	// Output occlusion
	outColor = occlusion;
}

vec3 ReconstructPosition(vec2 coords)
{
	// Get depth
	float depth = texture(gDepth, coords).r;
	// Invert projection
	vec4 projectedPos = invProj * (vec4(coords, depth, 1.0f) * 2.0f - 1.0f);
	// Invert view
	vec3 viewPos = projectedPos.xyz / projectedPos.w;
	// Return
	return viewPos;
}

// FIXME: Remove conditionals
vec3 UnpackNormal(vec2 pNormal)
{
	// Convert range of packed normal
	pNormal = pNormal * 2.0f - 1.0f;
	// Create new normal
	vec3 normal = vec3(pNormal.x, pNormal.y, 1.0f - abs(pNormal.x) - abs(pNormal.y));
	// Calculate flags
	float flag = max(-normal.z, 0.0f);
	// X check
	normal.x += normal.x >= 0.0f ? -flag : flag;
	// Y check
	normal.y += normal.y >= 0.0f ? -flag : flag;
	// Return normalised normal
	return normalize(normal); // FIXME: Convert to view space
}