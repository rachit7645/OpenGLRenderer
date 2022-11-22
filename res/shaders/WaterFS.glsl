#version 430 core

const float WAVE_STRENGTH   = 0.04f;
const float REFRACT_AMOUNT  = 0.7f;
const float SHINE_DAMPER    = 16.0f;
const float REFLECTIVITY    = 0.2f;
const float MIN_SPECULAR    = 0.0f;
const float NORMAL_FACTOR_Y = 0.1f;
const int   MAX_LIGHTS      = 4;

layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
	vec4 cameraPos;
	vec4 resolution;
};

in vec4 clipSpace;
in vec3 unitCameraVector;
in vec3 unitLightVector;
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
vec3  CalculateSpecular(vec3 unitNormal);

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

	refractTxCoords   += totalDistortion;
	refractTxCoords   = clamp(refractTxCoords, 0.001f, 0.999f);

	vec4 reflectColor = texture(reflectionTx, reflectTxCoords);
	vec4 refractColor = texture(refractionTx, refractTxCoords);

	vec3  unitNormal    = CalculateNormal(distortedCoords);
	float refractFactor = CalculateRefractiveFactor(unitNormal);
	vec3  specular      = CalculateSpecular(unitNormal);

	outColor = mix(reflectColor, refractColor, refractFactor);
	outColor = mix(outColor, vec4(0.0f, 0.3f, 0.5f, 0.0f), 0.2f);
	outColor = outColor + vec4(specular, 0.0f);
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
	vec4 normalColor = texture(normalMap, distortedCoords);
	return normalize(vec3
	(
		normalColor.r * 2.0f - 1.0f,
		normalColor.b * NORMAL_FACTOR_Y,
		normalColor.g * 2.0f - 1.0f
	));
}

float CalculateRefractiveFactor(vec3 unitNormal)
{
	float refractFactor = dot(unitCameraVector, unitNormal);
	refractFactor       = pow(refractFactor, REFRACT_AMOUNT);
	return clamp(refractFactor, 0.0f, 1.0f);
}

vec3 CalculateSpecular(vec3 unitNormal)
{
	vec3 lightDirection  = unitLightVector;
	vec3 halfwayDir      = normalize(lightDirection + unitCameraVector);
	float specularFactor = dot(halfwayDir, unitNormal);
	specularFactor       = max(specularFactor, MIN_SPECULAR);
	float dampedFactor   = pow(specularFactor, SHINE_DAMPER);
	return dampedFactor * REFLECTIVITY * vec3(1.0f);
}