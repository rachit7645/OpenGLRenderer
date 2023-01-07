#version 430 core

const float WAVE_STRENGTH  = 0.04f;
const float REFRACT_AMOUNT = 0.7f;

in vec4 worldPos;
in vec4 clipSpace;
in vec3 unitNormal;
in vec3 unitCameraVector;
in vec2 txCoords;

uniform sampler2D reflectionTx;
uniform sampler2D refractionTx;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;

uniform float moveFactor;

out vec4 outColor;

vec2  CalculateDistortedCoords();
vec2  CalculateTotalDistortion(vec2 distortedCoords);
vec3  CalculateNormal(vec2 distortedCoords);
float CalculateRefractiveFactor(vec3 unitNormal);

void main()
{
	vec2 NDC             = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	vec2 reflectTxCoords = vec2(NDC.x, -NDC.y);
	vec2 refractTxCoords = vec2(NDC.x, NDC.y);

	vec2 distortedCoords = CalculateDistortedCoords();
	vec2 totalDistortion = CalculateTotalDistortion(distortedCoords);

	reflectTxCoords   += totalDistortion;
	reflectTxCoords.x = clamp(reflectTxCoords.x, 0.001f, 0.999f);
	reflectTxCoords.y = clamp(reflectTxCoords.y, -0.999f, -0.001f);

	refractTxCoords += totalDistortion;
	refractTxCoords  = clamp(refractTxCoords, 0.001f, 0.999f);

	vec4 reflectColor = texture(reflectionTx, reflectTxCoords);
	vec4 refractColor = texture(refractionTx, refractTxCoords);

	vec3  unitNormal    = CalculateNormal(distortedCoords);
	float refractFactor = CalculateRefractiveFactor(unitNormal);

	outColor = mix(reflectColor, refractColor, refractFactor);
	outColor = mix(outColor, vec4(0.0f, 0.3f, 0.5f, 0.0f), 0.2f);
}

vec2 CalculateTotalDistortion(vec2 distortedCoords)
{
	vec2 totalDistortion = texture(dudvMap, distortedCoords).rg * 2.0f - 1.0f;
	return totalDistortion * WAVE_STRENGTH;
}

vec2 CalculateDistortedCoords()
{
	vec2 distortedCoords = texture(dudvMap, vec2(txCoords.x + moveFactor, txCoords.y)).rg * 0.1f;
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
	vec3 N   = unitNormal;
	vec3 T   = normalize(Q1 * st2.t - Q2 * st1.t);
	vec3 B   = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	// Return world space normal
	return normalize(TBN * tangentNormal);
}

float CalculateRefractiveFactor(vec3 unitNormal)
{
	float refractFactor = dot(unitCameraVector, unitNormal);
	refractFactor       = pow(refractFactor, REFRACT_AMOUNT);
	return clamp(refractFactor, 0.0f, 1.0f);
}