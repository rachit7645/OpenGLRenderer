#version 450 core

// Constants
const float WAVE_STRENGTH  = 0.04f;
const float REFRACT_AMOUNT = 0.7f;

// Vertex inputs
in vec4 worldPos;
in vec4 clipSpace;
in vec3 unitNormal;
in vec3 unitCameraVector;
in vec2 txCoords;

// Texture inputs
uniform sampler2D reflectionTx;
uniform sampler2D refractionTx;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;

// Other uniforms
uniform float moveFactor;

// Fragment outputs
layout (location = 0) out vec4 outColor;

// Water functions
vec2  CalculateDistortedCoords();
vec2  CalculateTotalDistortion(vec2 distortedCoords);
vec3  CalculateNormal(vec2 distortedCoords);
float CalculateRefractiveFactor(vec3 unitNormal);

// Entry point
void main()
{
	// Projective texture mapping
	vec2 NDC             = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	vec2 reflectTxCoords = vec2(NDC.x, -NDC.y);
	vec2 refractTxCoords = vec2(NDC.x,  NDC.y);

	// Calculated distortion
	vec2 distortedCoords = CalculateDistortedCoords();
	vec2 totalDistortion = CalculateTotalDistortion(distortedCoords);

	// Calculate reflection texture coordinates
	reflectTxCoords  += totalDistortion;
	reflectTxCoords.x = clamp(reflectTxCoords.x,  0.001f,  0.999f);
	reflectTxCoords.y = clamp(reflectTxCoords.y, -0.999f, -0.001f);

	// Calculate refraction texture coordinates
	refractTxCoords += totalDistortion;
	refractTxCoords  = clamp(refractTxCoords, 0.001f, 0.999f);

	// Sample reflection and refraction
	vec4 reflectColor = texture(reflectionTx, reflectTxCoords);
	vec4 refractColor = texture(refractionTx, refractTxCoords);

	// Fresnel effect
	vec3  unitNormal    = CalculateNormal(distortedCoords);
	float refractFactor = CalculateRefractiveFactor(unitNormal);

	// Mix colors
	outColor = mix(reflectColor, refractColor, refractFactor);
	// Mix blueish color
	outColor = mix(outColor, vec4(0.0f, 0.3f, 0.5f, 0.0f), 0.2f);
}

vec2 CalculateTotalDistortion(vec2 distortedCoords)
{
	// Sample DUDV map
	vec2 totalDistortion = texture(dudvMap, distortedCoords).rg * 2.0f - 1.0f;
	// Multiply by wave strength
	return totalDistortion * WAVE_STRENGTH;
}

vec2 CalculateDistortedCoords()
{
	// Sample DUDV map
	vec2 distortedCoords = texture(dudvMap, vec2(txCoords.x + moveFactor, txCoords.y)).rg * 0.1f;
	// Move texture coordinates
	return txCoords + vec2(distortedCoords.x, distortedCoords.y + moveFactor);
}

vec3 CalculateNormal(vec2 distortedCoords)
{
	// Get normal
	vec3 normal = texture(normalMap, distortedCoords).rgb;
	// Tangent normal from normal map
	vec3 tangentNormal = normal * 2.0f - 1.0f;

	// Take the deriviatives
	vec3 Q1  = dFdx(worldPos.xyz);
	vec3 Q2  = dFdy(worldPos.xyz);
	vec2 st1 = dFdx(txCoords);
	vec2 st2 = dFdy(txCoords);

	// Calculate TBN matrix
	vec3 N   =  unitNormal;
	vec3 T   =  normalize(Q1 * st2.t - Q2 * st1.t);
	vec3 B   = -normalize(cross(N, T));
	mat3 TBN =  mat3(T, B, N);

	// Return world space normal
	return normalize(TBN * tangentNormal);
}

float CalculateRefractiveFactor(vec3 unitNormal)
{
	// Caluclate dot product
	float refractFactor = dot(unitCameraVector, unitNormal);
	// Raise to the power of refraction amount
	refractFactor = pow(refractFactor, REFRACT_AMOUNT);
	// Clamo between 0 and 1 and retrun
	return clamp(refractFactor, 0.0f, 1.0f);
}