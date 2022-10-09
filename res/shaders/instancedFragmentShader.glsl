#version 430 core

// TODO: Add back muliple lights

const float AMBIENT_STRENGTH = 0.2f;
const float MIN_SPECULAR     = 0.0f;
const int   MAX_LIGHTS       = 4;

const float PCF_COUNT    = 3.5f;
const float TOTAL_TEXELS = (PCF_COUNT * 2.0f - 1.0f) * (PCF_COUNT * 2.0f - 1.0f);

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
	mat4  lightProj;
	mat4  lightView;
};

layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
	vec4 skyColor;
	vec4 cameraPos;
};

in float visibility;
in vec2  txCoords;
in vec3  unitNormal;
in vec3  unitCameraVector;
in vec3  unitLightVector[MAX_LIGHTS];
in vec4  worldPosition;
in vec4  lightSpacePos;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;

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

float CalculateShadow()
{
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
	projCoords      = projCoords * 0.5f + 0.5f;

	float currentDepth = projCoords.z;
	float closestDepth = texture(shadowMap, projCoords.xy).r;

	vec3  lightDir = unitLightVector[0];
	float bias     = max(0.05f * (1.0f - dot(unitNormal, lightDir)), 0.005f);

	float shadow    = 0.0f;
	vec2  texelSize = 1.0f / textureSize(shadowMap, 0);

	for (float x = -PCF_COUNT; x <= PCF_COUNT; ++x)
	{
		for (float y = -PCF_COUNT; y <= PCF_COUNT; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow        += WhenGreater(vec4(currentDepth - bias), vec4(pcfDepth)).x;
		}
	}

	shadow /= TOTAL_TEXELS;
	return shadow * WhenLesser(vec4(projCoords.z), vec4(1.0f)).x;
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