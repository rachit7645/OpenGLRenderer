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

struct SpotLight
{
	vec4 position;
	vec4 color;
	vec4 intensity;
	vec4 attenuation;
	vec4 direction;
	vec4 cutOff;
};

layout(std140, binding = 1) uniform Lights
{
	// Directional lights
	int      numDirLights;
	DirLight dirLights[MAX_LIGHTS];
	// Point lights
	int        numPointLights;
	PointLight pointLights[MAX_LIGHTS];
	// Spot Lights
	int numSpotLights;
	SpotLight spotLights[MAX_LIGHTS];
};

in vec2 txCoords;
in vec3 unitNormal;
in vec4 worldPosition;

in vec3 unitDirLightVector[MAX_LIGHTS];
in vec3 unitPointLightVector[MAX_LIGHTS];
in vec3 unitSpotLightVector[MAX_LIGHTS];
in vec3 unitSpotLightDirVector[MAX_LIGHTS];

uniform sampler2D diffuseTexture;

out vec4 outColor;

vec4  CalculateAmbient();
vec4  CalculateDiffuse(int index, vec3 toLightVector, vec3 lightColor);
float CalculateAttFactor(int index, vec3 position, vec3 attenuation);
float CalculateSpotIntensity(int index);

void main()
{
	// Fetch diffuse color
	vec4 diffColor = texture(diffuseTexture, txCoords);

	// Allocate memory
	vec4 ambient = vec4(0.0f);
	vec4 diffuse = vec4(0.0f);

	for (int i = 0; i < numDirLights; ++i)
	{
		// Calculate ambient
		ambient += CalculateAmbient() * diffColor;
		// Calculate diffuse
		diffuse += CalculateDiffuse(i, unitDirLightVector[i].xyz, dirLights[i].color.rgb) * diffColor;
	}

	for (int i = 0; i < numPointLights; ++i)
	{
		// Calculate attenuation
		float attFactor = CalculateAttFactor(i, pointLights[i].position.xyz, pointLights[i].attenuation.xyz);
		// Calculate ambient
		ambient += CalculateAmbient() * diffColor * attFactor;
		// Calculate diffuse
		diffuse += CalculateDiffuse(i, unitPointLightVector[i].xyz, pointLights[i].color.rgb) * diffColor * attFactor;
	}

	for (int i = 0; i < numSpotLights; ++i)
	{
		// Calculate attenuation
		float attFactor = CalculateAttFactor(i, spotLights[i].position.xyz, spotLights[i].attenuation.xyz);
		// Calculate intensity
		float intensity = CalculateSpotIntensity(i);
		// Calculate ambient
		ambient += CalculateAmbient() * diffColor * attFactor;
		// Calculate diffuse
		diffuse += CalculateDiffuse(i, unitSpotLightVector[i].xyz, spotLights[i].color.rgb) * diffColor * attFactor * intensity;
	}

	outColor = ambient + diffuse;
}

vec4 CalculateAmbient()
{
	return vec4(AMBIENT_STRENGTH * vec3(0.2f), 1.0f);
}

vec4 CalculateDiffuse(int index, vec3 toLightVector, vec3 lightColor)
{
	float nDot1      = dot(unitNormal, toLightVector);
	float brightness = max(nDot1, 0.0f);
	vec3  diffuse     = brightness * lightColor;
	// Return
	return vec4(diffuse, 1.0f);
}

float CalculateAttFactor(int index, vec3 position, vec3 attenuation)
{
	float distance  = length(position - worldPosition.xyz);
	float attFactor = attenuation.x + (attenuation.y * distance) + (attenuation.z * distance * distance);
	attFactor       = 1.0f / attFactor;
	// Return
	return attFactor;
}

float CalculateSpotIntensity(int index)
{
	float theta     = dot(unitSpotLightVector[index], unitSpotLightDirVector[index]);
	float epsilon   = spotLights[index].cutOff.x - spotLights[index].cutOff.y;
	float intensity = smoothstep(0.0f, 1.0f, (theta - spotLights[index].cutOff.y) / epsilon);
	// Return
	return intensity;
}