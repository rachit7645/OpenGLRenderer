#version 430 core

// TODO: Add back muliple lights

const float AMBIENT_STRENGTH = 0.2f;
const float MIN_SPECULAR     = 0.0f;
const int   MAX_LIGHTS       = 4;

const int   MAX_LAYER_COUNT = 16;
const float MIN_BIAS        = 0.005f;
const float MAX_BIAS        = 0.05f;
const float SHADOW_AMOUNT   = 0.5f;
const float BIAS_MODIFIER   = 0.5f;
const float PCF_COUNT       = 3.5f;
const float TOTAL_TEXELS    = (PCF_COUNT * 2.0f - 1.0f) * (PCF_COUNT * 2.0f - 1.0f);

struct Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 attenuation;
};

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140, binding = 1) uniform Lights
{
	Light lights[MAX_LIGHTS];
};

layout(std140, binding = 2) uniform Shared
{
	vec4  clipPlane;
	vec4  skyColor;
	vec4  cameraPos;
	float farPlane;
};

layout (std140, binding = 4) uniform ShadowBuffer
{
	int   cascadeCount;
	mat4  shadowMatrices[MAX_LAYER_COUNT];
	float cascadeDistances[MAX_LAYER_COUNT];
};

in float visibility;
in vec2  txCoords;
in vec3  unitNormal;
in vec3  unitCameraVector;
in vec3  unitLightVector[MAX_LIGHTS];
in vec4  worldPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform sampler2DArray shadowMap;

uniform float shineDamper;
uniform float reflectivity;

out vec4 outColor;

vec4 WhenNotEqual(vec4 x, vec4 y);
vec4 WhenGreater(vec4 x, vec4 y);
vec4 WhenLesser(vec4 x, vec4 y);

float CalculateAttFactor(int index);
vec4  CalculateAmbient(int index);
vec4  CalculateDiffuse(int index);
vec4  CalculateSpecular(int index);

int   GetCurrentLayer();
float CalculateBias(int layer);
float CalculateShadow();

void main()
{
	vec4 diffuseColor  = texture(diffuseTexture,  txCoords);
	vec4 specularColor = texture(specularTexture, txCoords);

	float attFactor = CalculateAttFactor(0);
	vec4 ambient    = CalculateAmbient(0)  * diffuseColor  * attFactor;
	vec4 diffuse    = CalculateDiffuse(0)  * diffuseColor  * attFactor * (1.0f - CalculateShadow());
	vec4 specular   = CalculateSpecular(0) * specularColor * attFactor * WhenNotEqual(diffuse, vec4(0.0f));

	outColor = ambient + diffuse + specular;
	// Mix with fog colour
	outColor = mix(skyColor, outColor, visibility);
}

float CalculateAttFactor(int index)
{
	vec4  ATT       = lights[index].attenuation;
	float distance  = length(lights[index].position.xyz - worldPosition.xyz);
	float attFactor = ATT.x + (ATT.y * distance) + (ATT.z * distance * distance);
	return 1.0f / attFactor;
}

vec4 CalculateAmbient(int index)
{
	return vec4(AMBIENT_STRENGTH * lights[index].ambient.rgb, 1.0f);
}

vec4 CalculateDiffuse(int index)
{
	float nDot1      = dot(unitNormal, unitLightVector[index]);
	float brightness = max(nDot1, 0.0f);
	return vec4(brightness * lights[index].diffuse.rgb, 1.0f);
}

vec4 CalculateSpecular(int index)
{
	vec3 lightDirection  = unitLightVector[index];
	vec3 halfwayDir      = normalize(lightDirection + unitCameraVector);
	float specularFactor = dot(halfwayDir, unitNormal);
	specularFactor       = max(specularFactor, MIN_SPECULAR);
	float dampedFactor   = pow(specularFactor, shineDamper);
	return vec4(dampedFactor * reflectivity * lights[index].specular.rgb, 1.0f);
}

// FIXME: Remove branching
int GetCurrentLayer()
{
	vec4  viewPosition = viewMatrix * worldPosition;
	float depthValue   = abs(viewPosition.z);

	int layer = -1;
	for (int i = 0; i < cascadeCount; ++i)
	{
		if (depthValue < cascadeDistances[i])
		{
			layer = i;
			break;
		}
	}

	if (layer == -1)
	{
		layer = cascadeCount;
	}

	return layer;
}

// FIXME: Remove branching
float CalculateBias(int layer)
{
	float bias = max(MAX_BIAS * (1.0f - dot(unitNormal, unitLightVector[0])), MIN_BIAS);

	if (layer == cascadeCount)
	{
		bias *= 1.0f / (farPlane * BIAS_MODIFIER);
	}
	else
	{
		bias *= 1.0f / (cascadeDistances[layer] * BIAS_MODIFIER);
	}

	return bias;
}

float CalculateShadow()
{
	int layer = GetCurrentLayer();

	vec4 lightSpacePos = shadowMatrices[layer] * worldPosition;
	vec3 projCoords    = lightSpacePos.xyz / lightSpacePos.w;
	projCoords         = projCoords * 0.5f + 0.5f;
	float currentDepth = projCoords.z;
	float bias         = CalculateBias(layer);

	float shadow    = 0.0f;
	vec2  texelSize = 1.0f / vec2(textureSize(shadowMap, 0));

	for (float x = -PCF_COUNT; x <= PCF_COUNT; ++x)
	{
		for (float y = -PCF_COUNT; y <= PCF_COUNT; ++y)
		{
			float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
			shadow        += SHADOW_AMOUNT * WhenGreater(vec4(currentDepth - bias), vec4(pcfDepth)).x;
		}
	}

	shadow /= TOTAL_TEXELS;
	return shadow * WhenLesser(vec4(currentDepth), vec4(1.0f)).x;
}

// Branchless implementation of
// if (x != y)
//	 return 1;
// else
// 	 return 0;
vec4 WhenNotEqual(vec4 x, vec4 y)
{
	return abs(sign(x - y));
}

// Branchless implementation of
// if (x > y)
//	 return 1;
// else
// 	 return 0;
vec4 WhenGreater(vec4 x, vec4 y)
{
	return max(sign(x - y), 0.0f);
}

// Branchless implementation of
// if (x < y)
//	 return 1;
// else
// 	 return 0;
vec4 WhenLesser(vec4 x, vec4 y)
{
	return max(sign(y - x), 0.0f);
}