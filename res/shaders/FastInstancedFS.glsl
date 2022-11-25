#version 430 core

const float AMBIENT_STRENGTH = 0.6f;
const int   MAX_LIGHTS       = 4;

struct DirLight
{
	vec4 position;
	vec4 color;
	vec4 intensity;
};

struct PointLight
{
	vec4 position;
	vec4 color;
	vec4 intensity;
	vec4 attenuation;
};

layout(std140, binding = 1) uniform Lights
{
	// Directional lights
	int      numDirLights;
	DirLight dirLights[MAX_LIGHTS];
	// Point lights
	int        numPointLights;
	PointLight pointLights[MAX_LIGHTS];
};

in vec2 txCoords;
in vec3 unitNormal;
in vec3 unitDirLightVector[MAX_LIGHTS];
in vec3 unitPointLightVector[MAX_LIGHTS];
in vec4 worldPosition;

uniform sampler2D diffuseTexture;

out vec4 outColor;

vec4  CalculateAmbientAll();
float CalculateAttFactorPoint(int index);
vec4  CalculateDiffuseDir(int index);
vec4  CalculateDiffusePoint(int index);

void main()
{
	vec4 diffColor = texture(diffuseTexture, txCoords);

	vec4 ambient = vec4(0.0f);
	vec4 diffuse = vec4(0.0f);

	for (int i = 0; i < numDirLights; ++i)
	{
		ambient += CalculateAmbientAll()  * diffColor;
		diffuse += CalculateDiffuseDir(i) * diffColor;
	}

	for (int i = 0; i < numPointLights; ++i)
	{
		float attFactor = CalculateAttFactorPoint(i);
		ambient        += CalculateAmbientAll()    * diffColor * attFactor;
		diffuse        += CalculateDiffusePoint(i) * diffColor * attFactor;
	}

	outColor = ambient + diffuse;
}

vec4 CalculateAmbientAll()
{
	return vec4(AMBIENT_STRENGTH * vec3(0.2f), 1.0f);
}

float CalculateAttFactorPoint(int index)
{
	vec4  ATT       = pointLights[index].attenuation;
	float distance  = length(pointLights[index].position.xyz - worldPosition.xyz);
	float attFactor = ATT.x + (ATT.y * distance) + (ATT.z * distance * distance);
	return 1.0f / attFactor;
}

vec4 CalculateDiffuseDir(int index)
{
	float nDot1      = dot(unitNormal, unitDirLightVector[index]);
	float brightness = max(nDot1, 0.0f);
	return vec4(brightness * dirLights[index].color.rgb, 1.0f);
}

vec4 CalculateDiffusePoint(int index)
{
	float nDot1      = dot(unitNormal, unitPointLightVector[index]);
	float brightness = max(nDot1, 0.0f);
	return vec4(brightness * pointLights[index].color.rgb, 1.0f);
}