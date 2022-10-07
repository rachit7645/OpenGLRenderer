#version 430 core

const float AMBIENT_STRENGTH = 0.2f;
const float MIN_SPECULAR     = 0.0f;
const int   MAX_LIGHTS       = 4; 

struct Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 attenuation;
};

struct Instance
{
	mat4 modelMatrix;
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
	vec4 diffuse    = CalculateDiffuse(0)  * diffuseColor  * attFactor;
	vec4 specular   = CalculateSpecular(0) * specularColor * attFactor * WhenNotEqual(diffuse, vec4(0.0f));

	outColor = ambient + (diffuse + specular) * (1.0f - CalculateShadow());
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

	if (projCoords.z > 1.0)
	{
		return 0.0f;
	}

	float currentDepth = projCoords.z;
	float closestDepth = texture(shadowMap, projCoords.xy).r;

	vec3  lightDir = unitLightVector[0];
	float bias     = max(0.05 * (1.0 - dot(unitNormal, lightDir)), 0.005);

	float shadow   = 0.0f;
	vec2 texelSize = 1.0f / textureSize(shadowMap, 0);

	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow        += currentDepth - bias > pcfDepth ? 1.0f : 0.0f;
		}
	}

	return shadow / 9.0f;
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