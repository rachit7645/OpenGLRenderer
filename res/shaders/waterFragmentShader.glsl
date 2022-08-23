#version 430 core

// FIXME: Ugly jagged edges
// Try making framebuffers MSAA

#define NEAR_PLANE frustum.x
#define FAR_PLANE  frustum.y

const float WAVE_STRENGTH         = 0.04f;
const float REFLECT_AMOUNT        = 0.7f;
const float SHINE_DAMPER          = 20.0f;
const float REFLECTIVITY          = 0.5f;
const float MIN_SPECULAR          = 0.0f;
const float EDGE_BLEND_FACTOR     = 1.2f;
const float DISTORT_BLEND_FACTOR  = 2.5f;
const float SPECULAR_BLEND_FACTOR = 1.4f;
const float NORMAL_FACTOR_Y       = 3.0f;
const int   MAX_LIGHTS            = 4;

struct Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 attenuation;
};

layout(std140, binding = 1) uniform Lights
{
	Light lights[MAX_LIGHTS];
};

layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
	vec4 skyColor;
	vec4 cameraPos;
	vec2 frustum;
};

in vec4 clipSpace;
in vec3 unitCameraVector;
in vec3 unitLightVector;
in vec2 txCoords;

uniform sampler2D reflectionTx;
uniform sampler2D refractionTx;
uniform sampler2D refractDepth;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;

uniform float moveFactor;

out vec4 outColor;

float LogNToLinear(float x, float min, float max);
float CalculateWaterDepth(vec2 refractTxCoords);
vec2  CalculateDistortedCoords();
vec2  CalculateTotalDistortion(vec2 distortedCoords, float waterDepth);
vec3  CalculateNormal(vec2 distortedCoords);
float CalculateRefractiveFactor(vec3 unitNormal);
vec3  CalculateSpecular(vec3 unitNormal, float waterDepth);

void main()
{
	vec2 NDC             = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	vec2 reflectTxCoords = vec2(NDC.x, -NDC.y);
	vec2 refractTxCoords = vec2(NDC.x, NDC.y);

	float waterDepth     = CalculateWaterDepth(refractTxCoords);
	vec2 distortedCoords = CalculateDistortedCoords();
	vec2 totalDistortion = CalculateTotalDistortion(distortedCoords, waterDepth);

	reflectTxCoords   += totalDistortion;
	reflectTxCoords.x = clamp(reflectTxCoords.x, 0.001f, 0.999f);
	reflectTxCoords.y = clamp(reflectTxCoords.y, -0.999f, -0.001f);

	refractTxCoords   += totalDistortion;
	refractTxCoords   = clamp(refractTxCoords, 0.001f, 0.999f);

	vec4 reflectColor = texture(reflectionTx, reflectTxCoords);
	vec4 refractColor = texture(refractionTx, refractTxCoords);

	vec3  unitNormal    = CalculateNormal(distortedCoords);
	float refractFactor = CalculateRefractiveFactor(unitNormal);
	vec3  specular      = CalculateSpecular(unitNormal, waterDepth);

	outColor   = mix(reflectColor, refractColor, refractFactor);
	outColor   = mix(outColor, vec4(0.0f, 0.3f, 0.5f, 0.0f), 0.2f);
	outColor   = outColor + vec4(specular, 0.0f);
	outColor.a = clamp(waterDepth / EDGE_BLEND_FACTOR, 0.0f, 1.0f);
}

float LogNToLinear(float x, float min, float max)
{
	return (2.0f * min * max) / (min + max - (2.0f * x - 1.0f) * (max - min));
}

vec2 CalculateTotalDistortion(vec2 distortedCoords, float waterDepth)
{
	vec2 totalDistortion = texture(dudvMap, distortedCoords).rg * 2.0f - 1.0f;
	return totalDistortion * WAVE_STRENGTH * clamp(waterDepth / DISTORT_BLEND_FACTOR, 0.0f, 1.0f);
}

vec2 CalculateDistortedCoords()
{
	vec2 distortedCoords = texture(dudvMap, vec2(txCoords.x + moveFactor, txCoords.y)).rg * 0.1f;
	return txCoords + vec2(distortedCoords.x, distortedCoords.y + moveFactor);
}

vec3 CalculateNormal(vec2 distortedCoords)
{
	vec4 normalColor = texture(normalMap, distortedCoords);
	return normalize(vec3
	(
		normalColor.r * 2.0f - 1.0f,
		normalColor.b * NORMAL_FACTOR_Y,
		normalColor.g * 2.0f - 1.0f
	));
}

float CalculateWaterDepth(vec2 refractTxCoords)
{
	float depth         = texture(refractDepth, refractTxCoords).r;
	float floorDistance = LogNToLinear(depth,          NEAR_PLANE, FAR_PLANE);
	float waterDistance = LogNToLinear(gl_FragCoord.z, NEAR_PLANE, FAR_PLANE);
	return floorDistance - waterDistance;
}

float CalculateRefractiveFactor(vec3 unitNormal)
{
	float refractFactor = dot(unitCameraVector, unitNormal);
	refractFactor       = pow(refractFactor, REFLECT_AMOUNT);
	return clamp(refractFactor, 0.0f, 1.0f);
}

vec3 CalculateSpecular(vec3 unitNormal, float waterDepth)
{
	vec3 lightDirection  = unitLightVector;
	vec3 halfwayDir      = normalize(lightDirection + unitCameraVector);
	float specularFactor = dot(halfwayDir, unitNormal);
	specularFactor       = max(specularFactor, MIN_SPECULAR);
	float dampedFactor   = pow(specularFactor, SHINE_DAMPER);
	vec3 specular        = dampedFactor * REFLECTIVITY * lights[0].specular.rgb;
	return specular * clamp(waterDepth / SPECULAR_BLEND_FACTOR, 0.0f, 1.0f);
}